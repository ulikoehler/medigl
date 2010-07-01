#define GL_GLEXT_PROTOTYPES



using namespace std;

#include "glwidget.h"
#include "shaders.h"

struct C4UBV3F
{
    unsigned char color[4];
    float vcoords[3];
};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    vboID = INT_MAX;
    //Initialize the transformation variables
    transformationMode = Rotate;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    xTrans = 0;
    yTrans = 0;
    zTrans = 0;

    zoomFactor = 1.0;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    if(vboID != INT_MAX)
    {
        glDeleteBuffers(1, &vboID);
    }
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(200, 200);
}

QSize GLWidget::sizeHint() const
{
    return QSize(1024, 1024);
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0); //Don't render fully transparent vertices
    glEnable(GL_TEXTURE_3D);
    //glEnable(GL_CULL_FACE);

    //Enable antialiasing
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //
    //GLSL shaders
    //

    //Compile the shader
    QSharedPointer<QGLShader> shader(new QGLShader(QGLShader::Vertex, this));
    if(!shader->compileSourceCode(vertexColorShaderSource))
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "The GLSL shader failed to compile!\n" + shader->log()).exec();
    }

    //Create a shader program from the shader
    shaderProgram = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram(this));
    if(!shaderProgram->addShader(shader.data()))
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to add the GLSL shader to the main shader program!").exec();
    }

    //Link the shader program
    if(!shaderProgram->link())
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to link the main shader program!").exec();
    }

    //Active=bind the shader program to the current GL context
    if(!shaderProgram->bind())
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to bind the main shader program!").exec();
    }
    shaderProgram->attributeLocation("density");
}

//Macro to adress virtual 3D-Adresses in one-dimensional space.
//The adressing is relative, so you have to add the adress of the beginning of a pre-reserved space with the correct size.
//This is no function but a macro to increase speed and make it easy to auto-unroll loops.
//If the compiler/language supports MAD operations (e.g. OpenCL), the code takes advantage of them
//Parameters:
// width: The width of the 3D space
// height: The height of the 3D space
// elem_width: The width of each element in the 3D space in bytes
// x,y,z: The x, y and z coordinates of the element to be adressed
#define REL_ADDR_3D(width, height, elem_width, x, y, z)	((elem_width * (x * width + y)) + (width * height * z * elem_width))

#define byte char //The same by definition (C standard)

void GLWidget::render3DTex()
{
    unsigned int texname;
    glGenTextures(1, &texname);
    glBindTexture(GL_TEXTURE_3D, texname);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    uint depth = images.size();
    const int bytesPerTexel = 4; //RGBA, 8 bit

    unsigned char* texels = (unsigned char*)malloc(width * height * depth * bytesPerTexel);
    if(texels == NULL) {cerr << "3D texture texel malloc failed";return;}

    for(uint z = 0; z < depth; z++)
    {
        FastImage* img = images[z];
        for(uint y = 0; y < height; y++)
        {
            for(uint x = 0; x < width; x++)
            {
                unsigned char gray = img->getGray(x,y);
                texels[REL_ADDR_3D(width, height, bytesPerTexel, x, y, z)]     = gray; //R
                texels[REL_ADDR_3D(width, height, bytesPerTexel, x, y, z) + 1] = gray; //G
                texels[REL_ADDR_3D(width, height, bytesPerTexel, x, y, z) + 2] = gray; //B
                texels[REL_ADDR_3D(width, height, bytesPerTexel, x, y, z) + 3] = gray; //A
            }
        }
        //delete img;
    }

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
    glBindTexture(GL_TEXTURE_3D, texname);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //Check if the image vector is empty
    if(images.empty())
    {
        return; //Nothing to do
    }
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    //render3DTex();

    //glutSolidCube(1.0);

    const float constantScaleFactor = 2.0;

    //Scale down so everything fits on the screen
    if(images.size() != 0) //Else: div by 0
    {
        float scaleFactor = constantScaleFactor * zoomFactor;
        glScalef(scaleFactor/width, scaleFactor/height, scaleFactor/(images.size()));
        //glScalef(0.01,0.01,0.01);
    }

    //Move the images to the middle of the screen
    glTranslatef(-0.5*width,-0.5*height,0);
    glTranslated(xTrans, yTrans, zTrans);

    renderPointCloud();

}

void GLWidget::renderPointCloud()
{
    //Scale the points. The scale is dependent on the zoom factor
    glPointSize(1.7 * zoomFactor);
    glBegin(GL_POINTS);
    for(uint z = 0; z < images.size(); z++)
    {
        FastImage* img = images[z];
        for(uint y = 0; y < height; y++)
        {
            for(uint x = 0; x < width; x++)
            {
                unsigned char gray = img->getGray(x,y);
                //glVertex3f(x/100.0,y/100.0,z/100.0);
                glVertex3i(x,y,z);
                glColor3b(gray,gray,gray);
            }
        }
        //delete img;
    }
    glEnd();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );
    glMatrixMode(GL_MODELVIEW);

    refillVBO();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    //The mouse buttons (drag-n-drop) make the image rotate
    if(transformationMode == Rotate)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);
        }
        else if (event->buttons() & Qt::RightButton)
        {
            setXRotation(xRot + 8 * dy);
            setZRotation(zRot + 8 * dx);
        }
    }
    else if(transformationMode == Translate)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            xTrans += dx;
            yTrans += dy;
        }
        else if (event->buttons() & Qt::RightButton)
        {
            xTrans += dx;
            zTrans += dy;
        }
    }
    lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch(key)
    {
        /*
         * Transformation events: Arrow keys
         */
    case Qt::Key_Left:
        {
            xTrans -= 100;
            cout << "XT";
            break;
        }
    case Qt::Key_Right:
        {
            xTrans -= 1;
            break;
        }
    case Qt::Key_Up:
        {
            yTrans += 1;
            break;
        }
    case Qt::Key_Down:
        {
            yTrans -= 1;
            break;
        }
    default: {event->ignore();break;}
    }
    //Re-render. Event if another key has been pressed, this should not be too expensive
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    const float deltaFactor = 0.002;
    zoomFactor += deltaFactor * delta;
    //If we would allow negative values, the image would be mirrored --> set a minimum zoom factor of 1.0
    if(zoomFactor < 1.0)
    {
        zoomFactor = 1.0;
    }
    updateGL();
}

void GLWidget::refillVBO()
{
    /*if(vboID != INT_MAX)
    {
        glDeleteBuffers(1, &vboID);
    }
    glGenBuffers(1, &vboID);

    //Writing the data to the client memory directly
    //would be to inefficient (too many calls)
    glBindBuffer(GL_ARRAY_BUFFER,vboID);

    C4UBV3F* data = new C4UBV3F[images.size() * width * height];

    for(uint z = 0; z < images.size(); z++)
    {
        QImage* img = images[z];
        for(uint y = 0; y < height; y++)
        {
            for(uint x = 0; x < width; x++)
            {
                unsigned char gray = qGray(img->pixel(x,y));
                //printf("x: %u y: %u z:%u\n",x,y,z);
                //std::cout << endl;
                data[x + width * y + z * (width * height)].color[0] = 255;//gray;
                data[x + width * y + z * (width * height)].color[1] = 255;//gray;
                data[x + width * y + z * (width * height)].color[2] = 255;//gray;
                data[x + width * y + z * (width * height)].color[3] = 255; //Alpha
                data[x + width * y + z * (width * height)].vcoords[0] = x;
                data[x + width * y + z * (width * height)].vcoords[1] = y;
                data[x + width * y + z * (width * height)].vcoords[2] = z;
            }
        }
        //delete img;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glEnableClientState(GL_VERTEX_ARRAY);
    glInterleavedArrays(GL_C4UB_V3F, sizeof(C4UBV3F), 0);
    glBufferData(GL_ARRAY_BUFFER, images.size() * width * height * sizeof(C4UBV3F), data, GL_STATIC_DRAW);*/

    cout << "Successfully loaded" << endl;
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
    {
        *angle += 360 * 16;
    }
    while (*angle > 360 * 16)
    {
        *angle -= 360 * 16;
    }
}
