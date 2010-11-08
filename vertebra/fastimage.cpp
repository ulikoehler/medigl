#include "fastimage.h"
#include <limits>

//Macro for adressing pixels int virtually 2d arrays
#define REL_ADDR_2D(width, x, y) (y * width + x)

FastImage::FastImage(QImage* img, bool enableGrayCache)
{
    this->grayCacheEnabled = enableGrayCache;
    int width = img->width();
    int height = img->height();
    this->width = img->width();
    this->height = img->height();
    this->colorData = new uint32_t[width * height];
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < width; y++)
        {
            colorData[REL_ADDR_2D(width, x, y)] = img->pixel(x, y);
        }
    }
    //Build the gray cache if enabled
    if(enableGrayCache)
    {
        grayData = new double[width * height];
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < width; y++)
            { 
                grayData[REL_ADDR_2D(width, x, y)] = (double)qGray(img->pixel(x, y)) / std::numeric_limits<unsigned char>::max();
            }
        }
    }
    else
    {
        grayData = 0;
    }
}

QImage* FastImage::getGrayQImage()
{
    QImage* img = new QImage(width, height, QImage::Format_RGB888);

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < width; y++)
        {
            int gray = 255 * grayData[REL_ADDR_2D(width, x, y)];
            int rgb = QColor(gray, gray, gray).rgb();
            img->setPixel(x,y,rgb);
        }
    }
    return img;
}

FastImage::FastImage(uint width, uint height, bool enableGrayCache)
{
    this->grayCacheEnabled = enableGrayCache;
    this->width = width;
    this->height = height;
    this->colorData = new uint32_t[width * height];
    this->grayData = new double[width * height];
}

void FastImage::setPixel(uint x, uint y, uint32_t val)
{
    colorData[REL_ADDR_2D(width, x, y)] = val;
    //Update the gray cache if needed
    if(grayCacheEnabled)
    {
        grayData[REL_ADDR_2D(width, x, y)] = qGray(val) / std::numeric_limits<uint32_t>::max(); //1.0 is white
    }
}

void FastImage::setGrayPixel(uint x, uint y, unsigned char val)
{
    //Update the gray cache if enabled
    if(grayCacheEnabled)
    {
        grayData[REL_ADDR_2D(width, x, y)] = (double)val / std::numeric_limits<unsigned char>::max(); //1.0 is white
    }
}

void FastImage::setGrayPixel(uint x, uint y, uint32_t val)
{
    //Update the gray cache if enabled
    if(grayCacheEnabled)
    {
        grayData[REL_ADDR_2D(width, x, y)] = (double)val / std::numeric_limits<uint32_t>::max(); //1.0 is white

    }
}

char FastImage::getGray(uint x, uint y)
{
    if(grayCacheEnabled)
    {
        return grayData[REL_ADDR_2D(width, x, y)] * 255;
    }
}

double FastImage::getGray32bit(uint x, uint y)
{
    if(grayCacheEnabled)
    {
        return grayData[REL_ADDR_2D(width, x, y)];
    }
}

FastImage::~FastImage()
{
    delete[] colorData;
    if(grayCacheEnabled)
    {
        delete[] grayData;
    }
}
