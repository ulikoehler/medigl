#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <QVector>
#include <QGLShader>
#include <memory>
#include <GL/gl.h>
#include <GL/glext.h>
#include <QtOpenGL>
#include <GL/glut.h>
#include <iostream>
#include "fastimage.h"

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    inline void updateImages(vector<FastImage*> imagesParam, uint width, uint height)
    {
        this->images = imagesParam;
        this->width = width;
        this->height = height;
        //refillVBO();
        updateGL();
    }

    inline void resetView()
    {
        xRot = 0;
        yRot = 0;
        zRot = 0;

        xTrans = 0;
        yTrans = 0;
        zTrans = 0;

        zoomFactor = 1;

        updateGL();
    }

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    enum TransformationMode
    {Translate, Rotate, Scale};

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
private:
    void refillVBO();
    void normalizeAngle(int *angle);

    void render3DTex();

    /**
     * Rendering function; has to be called inside paintGL() when all neccessary transformations have been done
     *
     */
    void renderPointCloud();

    vector<FastImage*> images;
    uint width;
    uint height;

    TransformationMode transformationMode;

    float xRot;
    float yRot;
    float zRot;

    int xTrans;
    int yTrans;
    int zTrans;

    float zoomFactor;

    QPoint lastPos;

    QSharedPointer<QGLShaderProgram> shaderProgram;

    bool dataChanged;
    GLuint vboID;
};

#endif
