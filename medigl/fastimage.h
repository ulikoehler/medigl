#ifndef FASTIMAGE_H
#define FASTIMAGE_H

#include <QImage>
#include <stdint.h>
#include <iostream>
using namespace std;

/**
 * Image wrapper class internally using arrays for fast read access
 * Optimized for read access
 * Compiling with -fno-strict-aliasing should make this class faster
 *
 * One instance of this class represents exactly one image.
 * FastImage can build a gray cache to be able to serve getGray() requests
 * very fast. This feature is enabled by default but can be disabled.
 * Users are not encouraged to change the data using setPixel(...).
 *
 * FastImage internally stores the data using 64 bit floating point numbers
 *
 * FastImage is optimized to process grayscale images - some functions only work on grayscale images
 *
 * The purpose of this class is to have a fast, scalable abstraction layer between
 * MediGL input data and OpenGL in order to be able to support a variety of image formats.
 */
class FastImage
{
public:
    /**
     * Creates a new FastImage instance from a QImage.
     * Uses the data from the QImage instance to fill the cache
     * \param img The image to process
     * \param enableGrayCache Whether to enable a separate gray cache
     */
    FastImage(QImage* img, bool enableGrayCache = true);
    /**
     * Creates a new FastImage instance with given width and height
     * but without any content.
     * Use setPixel(...) to set the pixels
     * \param width The widi of the new image
     * \param height The height of the new image
     * \param enableGrayCache Whether to enable a separate gray cache
     */
    FastImage(uint width, uint height, bool enableGrayCache = true);
    /**
     * Releases all memory occupied by this FastImage instance
     */
    ~FastImage();
    /**
     * Gets the RGBA value for a specific pixel in this FastImage instance.
     *
     * This function does NOT check if the x and y parameters are in the bounds
     * of this FastImage instance for sake of performance
     */
    uint32_t getRgba(uint x, uint y);
    /**
     * Gets the gray value (8 bit) for specific x and y pixel coordinates.
     * The request is served from the gray cache if it has been enabled for this instance
     */
    char getGray(uint x, uint y);
    /**
     * Sets a pixel in this FastImage instance
     * to a specific value and updates the gray cache if it is enabled
     * \param x The x coordinate of the pixel to set
     * \param y The x coordinate of the pixel to set
     * \param val The RGBA value to set the pixel to
     */
    void setPixel(uint x, uint y, uint32_t val);
    /**
     * Sets a pixel to a specific gray value. The color buffer is not affected.
     * \param x The x coordinate of the pixel to set
     * \param y The x coordinate of the pixel to set
     * \param val The grayscale value to set the pixel to
     */
    void setGrayPixel(uint x, uint y, unsigned char val);
    /**
     * Sets a pixel to a specific gray value. The color buffer is not affected.
     * \param x The x coordinate of the pixel to set
     * \param y The x coordinate of the pixel to set
     * \param val The 32-bit grayscale value to set the pixel to
     */
    void setGrayPixel(uint x, uint y, uint32_t val);
    /**
     * Performs a constrast spreading on the gray data of this FastImage.
     *
     * The algorithm used has a linear complexity
     *
     * Note: The color data is not affected!
     */
    void spreadContrast();
    /**
     * \return The width of this image
     */
    uint getWidth()
    {
        return width;
    }
    /**
     * \return The height of this image
     */
    uint getHeight()
    {
        return height;
    }

protected:
    uint width, height;
    bool grayCacheEnabled;
    uint32_t* colorData;
    double* grayData;
};

#endif // FASTIMAGE_H
