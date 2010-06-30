#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector>
#include <QGLShader>
#include <memory>
#include "fastimage.h"

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateImages(vector<FastImage*> imagesParam, uint width, uint height)
    {
        this->images = imagesParam;
        this->width = width;
        this->height = height;
        //refillVBO();
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
private:
    void refillVBO();
    void normalizeAngle(int *angle);

    void render3DTex();

    vector<FastImage*> images;
    uint width;
    uint height;

    TransformationMode transformationMode;

    int xRot;
    int yRot;
    int zRot;

    int xTrans;
    int yTrans;
    int zTrans;

    int xScale;
    int yScale;
    int zScale;

    QPoint lastPos;

    QSharedPointer<QGLShaderProgram> shaderProgram;

    bool dataChanged;
    GLuint vboID;
};

#endif
