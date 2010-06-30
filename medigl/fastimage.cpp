#include "fastimage.h"

//Macro for adressing pixels int virtually 2d arrays
#define REL_ADDR_2D(width, x, y) (x * width + y)

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
        grayData = new char[width * height];
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < width; y++)
            {
                grayData[REL_ADDR_2D(width, x, y)] = qGray(img->pixel(x, y));
            }
        }
    }
    else
    {
        grayData = 0;
    }
}

FastImage::FastImage(uint width, uint height, bool enableGrayCache)
{
    this->grayCacheEnabled = enableGrayCache;
    this->width = width;
    this->height = height;
    this->colorData = new uint32_t[width * height];
}

void FastImage::setPixel(uint x, uint y, uint32_t val)
{
    colorData[REL_ADDR_2D(width, x, y)] = val;
    //Update the gray cache if needed
    if(grayCacheEnabled)
    {
        grayData[REL_ADDR_2D(width, x, y)] = qGray(val);
    }
}

void FastImage::setGrayPixel(uint x, uint y, char val)
{
    //Use the Qt API to convert the gray value into RGB
    setPixel(x, y, qGray(qRgb(val, val, val)));
}

char FastImage::getGray(uint x, uint y)
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
