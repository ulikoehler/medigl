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

//If no rendering method is defined, define a default
#ifndef RENDER_3DTEXTURE
#ifndef RENDER_POINTS
#ifndef RENDER_LINES
#ifndef RENDER_2DTEXTURE
#define RENDER_3DTEXTURE
#endif
#endif
#endif
#endif


using namespace std;

/**
 * Enumeration defining the valid rendering methods.
 * See GLWidget.setRenderingMethod
 */
enum RenderingMethod {PointCloud, Lines, TextureBlending2D, Texture3D};

/**
 * MediGL OpenGL widget
 * Controls and the OpenGL IO, displays the rendered data,
 * reacts to user events and processes images
 *
 * Note that the
 *
 * This class uses a function pointer to select the rendering function.
 * This decision (which results in very complex syntax at some points) was made
 * because using the RenderingEnum method directly was extremely slow even if using a switch.
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    /**
     * Updates the image cache with new images (represented by a vector of FastImage pointers)
     * with a given width and height.
     *
     * The images must be checked for equal width and height before - the GLWidget class does not
     * check them for performance reasons.
     */
    inline void updateImages(vector<FastImage*> imagesParam, uint width, uint height)
    {
        this->images = imagesParam;
        this->width = width;
        this->height = height;
        updateGL();
    }

    /**
     * Resets rotation, translation and zoom and re-renders the data.
     */
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

    /**
     * Sets the z (depth) extent of the rendered image cuboid
     * 1.0 is the same as the maximum of with and height
     */
    inline void setZExtent(float newZExtent)
    {
        this->zExtent = newZExtent;
        updateGL();
    }

    QSize minimumSizeHint() const;
    QSize sizeHint() const;


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
    /**
     * Reacts to a mouse press event.
     * This is part of the rotation code which rotates the data when the user uses drag-and-drop
     */
    void mousePressEvent(QMouseEvent *event);
    /**
     * Reacts to a mouse move event.
     * This is part of the rotation code which rotates the data when the user uses drag-and-drop
     */
    void mouseMoveEvent(QMouseEvent *event);
    /**
     * Reacts to a mouse wheel event.
     * Mouse wheel events are translated into zoom factor changes.
     */
    void wheelEvent(QWheelEvent *);
public:
    /**
     * Reacts to a key event.
     * Key events are translated into translation commands.
     *
     * All translations are absolute and not dependent on the rotation.
     * The translation amount is dependent on the zoom factor.
     *
     * Left/right arrow keys: x coordinates
     * Up/down arrow keys: y coordinates
     * PageUp/PageDown: z coordinates
     * +/-: zoom
     */
    void keyPressEvent(QKeyEvent *);
    /**
     * Sets the rendering method for this GLWidget and updates the screen
     * (= re-renders the current view using the new method
     */
    void setRenderingMethod(RenderingMethod method);
private:
    void refillVBO();
    void normalizeAngle(int *angle);
    void incrZoomFactor(int delta);

    void build3dTexture();

    //
    //
    // Rendering functions
    //
    //
    void render3DTex();

    /**
     * Rendering function; has to be called inside paintGL() when all neccessary transformations have been done.
     * This rendering function uses the PointCloud algorithm: Each pixel is rendered as GLPoint.
     */
    void renderPointCloud();

    /**
     * Rendering function; has to be called inside paintGL() when all neccessary transformations have been done.
     * Uses lines in the Z (depth) dimensions to display the data.
     *
     * For this rendering method it's not neccessary to generate virtual padding images
     */
    void renderLines();

    /**
     * Rendering function; has to be called inside paintGL() when all neccessary transformations have been done.
     * Uses 2D textures with alpha blending to display the data.
     */
    void render2DTextures();

    GLuint* textures2d;

    //
    //
    // Filling image generation functions
    //
    //
    void generateFillersCPU(int num);

    //
    //
    // Fields
    //
    //

    /**
     * Function pointer to the rendering function (no return value, no arguments)
     */
    void (GLWidget::*renderingMethod)(void);

    vector<FastImage*> originalImages; //Without filling images
    vector<FastImage*> images;
    uint width;
    uint height;

#ifdef RENDER_3DTEXTURE
    uint texptr3d; //GL internal pointer, no pointer to the 3d texture in the local memory
#endif

    float xRot;
    float yRot;
    float zRot;

    float xTrans;
    float yTrans;
    float zTrans;

    float zoomFactor; //For zooming with +/- or the mouse wheel
    float zExtent;

    QPoint lastPos;

    QSharedPointer<QGLShaderProgram> shaderProgram;

    bool dataChanged;
    GLuint vboID;
};

#endif
