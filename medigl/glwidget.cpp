#define GL_GLEXT_PROTOTYPES //Required for shaders etc.
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
    xRot = 0;
    yRot = 0;
    zRot = 0;

    xTrans = 0;
    yTrans = 0;
    zTrans = 0;

    zoomFactor = 1.0;

    zExtent = 1.0;

    textures2d = 0;

    renderingMethod = PointCloud;
}

void GLWidget::setRenderingMethod(RenderingMethod method)
{
    this->renderingMethod = method; //Set the member variable being read from the GL rendering function
    updateGL(); //Update the screen
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
    glTranslated(0.0, 0.0, -10.0); //Move the content away from the user to make him able to see everything
    //Apply the rotation. Rotate around the center TODO

    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

    //render3DTex();

    //glutSolidCube(1.0);

    const float constantScaleFactor = 2.0;

    //Scale down so everything fits on the screen
    float scaleFactor = constantScaleFactor * zoomFactor;
    glScalef(scaleFactor/width, scaleFactor/height, scaleFactor/(images.size()));

    //Move the images to the middle of the screen
    glTranslatef(-0.5*width,-0.5*height,0);
    glTranslatef(xTrans, yTrans, zTrans);

    //Scale the z axis (z extent)
    glScalef(1,1,zExtent);


    switch(renderingMethod)
    {
        case PointCloud: {renderPointCloud();}
        case Lines: {renderLines();}
        case TextureBlending2D: {render2DTextures();}
        case Texture3D: {render3DTex();}
    }

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
                double gray = img->getGray32bit(x,y);
                //glVertex3f(x/100.0,y/100.0,z/100.0);
                glVertex3i(x,y,z);
                glColor3d(gray,gray,gray);
            }
        }
    }
    glEnd();
}

void GLWidget::renderLines()
{
    //Scale the points. The scale is dependent on the zoom factor
    for(uint y = 0; y < height; y++)
    {
        for(uint x = 0; x < width; x++)
        {
            glBegin(GL_LINE_STRIP);
            for(uint z = 0; z < images.size(); z++)
            {
                double gray = images[z]->getGray32bit(x,y);
                glVertex3i(x,y,z);
                glColor3d(gray,gray,gray);
            }
            glEnd();
        }
    }
}

void GLWidget::render2DTextures()
{
    //Scale the points. The scale is dependent on the zoom factor
    for(uint z = 0; z < images.size(); z++)
    {
        FastImage* img = images[z];
        bindTexture(*(img->getGrayQImage()));

        glBegin(GL_TRIANGLE_STRIP);
                 glTexCoord2f(0,1); glVertex2f(-1,1); //lo
                 glTexCoord2f(0,0); glVertex2f(-1,-1); //lu
                 glTexCoord2f(1,1); glVertex2f(1,1);  //ru
                 glTexCoord2f(1,0); glVertex2f(1,-1); //ro
        glEnd();
    }
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
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    const float rotationFactor = 0.5;

    //The mouse buttons (drag-n-drop) cause the image to rotate
    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(xRot + rotationFactor * dy/zoomFactor);
        setYRotation(yRot + rotationFactor * dx/zoomFactor);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(xRot + rotationFactor * dy/zoomFactor);
        setZRotation(zRot + rotationFactor * dx/zoomFactor);
    }
    lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //To make the user able to translate when zoomed in,
    //we have to generate the translation amount of a constant
    //factor and the zoom factor
    const int coefficient = 10;
    float translationAmount = coefficient / zoomFactor;
    int key = event->key();
    switch(key)
    {
        /*
         * Transformation events: Arrow keys + PgUp/PgDown
         */
    case Qt::Key_Left:
        {
            xTrans -= translationAmount;
            break;
        }
    case Qt::Key_Right:
        {
            xTrans -= translationAmount;
            break;
        }
    case Qt::Key_Up:
        {
            yTrans += translationAmount;
            break;
        }
    case Qt::Key_Down:
        {
            yTrans -= translationAmount;
            break;
        }
    case Qt::Key_PageDown:
        {
            zTrans -= translationAmount;
            break;
        }
    case Qt::Key_PageUp:
        {
            zTrans += translationAmount;
            break;
        }
        /*
         * Zoom events: +/-
         */
    case Qt::Key_Plus:
        {
            incrZoomFactor(coefficient);
            break;
        }
    case Qt::Key_Minus:
        {
            incrZoomFactor(coefficient);
            break;
        }
    default: {event->ignore();break;}
    }
    //Re-render. Event if another key has been pressed, this should not be too expensive
    updateGL();
}

void GLWidget::incrZoomFactor(int delta)
{
    const float deltaFactor = 0.002;
    zoomFactor += deltaFactor * delta;
    //If we would allow negative values, the image would be mirrored --> set a minimum zoom factor of 1.0
    if(zoomFactor < 1.0)
    {
        zoomFactor = 1.0;
    }
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->delta();
    incrZoomFactor(delta);
}

void GLWidget::normalizeAngle(int *angle)
{
    *angle = (*angle) % 360;
}
