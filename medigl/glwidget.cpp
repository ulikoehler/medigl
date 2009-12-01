#define GL_GLEXT_PROTOTYPES

#include <QtGui>
#include <GL/gl.h>
#include <GL/glext.h>
#include <QtOpenGL>
#include <iostream>

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
    xRot = 0;
    yRot = 0;
    zRot = 0;
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
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
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
    glEnable(GL_CULL_FACE);


    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glAlphaFunc(GL_GREATER, 0.0);

    QSharedPointer<QGLShader> shader(new QGLShader(QGLShader::Vertex, this));
    if(!shader->compileSourceCode(vertexColorShaderSource))
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "The GLSL shader failed to compile!\n" + shader->log()).exec();
    }

    shaderProgram = QSharedPointer<QGLShaderProgram>(new QGLShaderProgram(this));
    if(!shaderProgram->addShader(shader.data()))
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to add the GLSL shader to the main shader program!").exec();
    }

    if(!shaderProgram->link())
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to link the main shader program!").exec();
    }

    if(!shaderProgram->bind())
    {
        QMessageBox(QMessageBox::Critical, "Shader failure", "Failed to bind the main shader program!").exec();
    }
    shaderProgram->attributeLocation("density");
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    /*glBegin(GL_QUADS);
    glVertex3f(-1,-1,1);
    glVertex3f(-1,1,1);
    glVertex3f(1,1,1);
    glVertex3f(1,-1,1);
    glEnd();*/

    glPointSize(5.0);

    //Scale down so everything fits on the screen
    if(images.size() != 0) //Else: div by 0
    {
        glScalef(1.0/width,1.0/height,1.0/(images.size()));
        //glScalef(0.01,0.01,0.01);
    }

    //Move the images to the middle of the screen
    glTranslatef(-0.5*width,-0.5*height,0);

    glBegin(GL_POINTS);
    for(uint z = 0; z < images.size(); z++)
    {
        QImage* img = images[z];
        for(uint y = 0; y < height; y++)
        {
            for(uint x = 0; x < width; x++)
            {
                unsigned char gray = qGray(img->pixel(x,y));
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
    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

    refillVBO();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
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
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
