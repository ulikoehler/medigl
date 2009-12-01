#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector>
#include <QGLShader>
#include <memory>

using namespace std;

//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateImages(vector<QImage* > imagesParam, uint width, uint height)
    {
        this->images = imagesParam;
        this->width = width;
        this->height = height;
        refillVBO();
    }

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
//! [0]

//! [1]
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
//! [1]

//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
//! [2]

//! [3]
private:
    void refillVBO();
    void quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2,
              GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4);
    void extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
    void normalizeAngle(int *angle);

    vector<QImage*> images;
    uint width;
    uint height;

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;

    QSharedPointer<QGLShaderProgram> shaderProgram;

    bool dataChanged;
    GLuint vboID;
};
//! [3]

#endif
