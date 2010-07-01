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
    this->grayData = new  double[width * height];
}

void FastImage::setPixel(uint x, uint y, uint32_t val)
{
    //#define VERBOSE_DEBUG
#ifdef VERBOSE_DEBUG
    cout << "WxH=" << width * height << " x "<<x << "y" << y << "   addr " << REL_ADDR_2D(width, x, y) << endl;
    cout << qRed(val) << "  " << qGreen(val) << "  " << qBlue(val) << "\n";
#endif
    colorData[REL_ADDR_2D(width, x, y)] = val;
    //Update the gray cache if needed
    if(grayCacheEnabled)
    {
        grayData[REL_ADDR_2D(width, x, y)] = qGray(val) / 255.0; //1.0 is white
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
        return grayData[REL_ADDR_2D(width, x, y)] * 255;
    }
}

void FastImage::spreadContrast()
{
    //Find the minimum and maximum values
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::min();
    for(int i = 0; i < width*height; i++)
    {
        minVal = min(minVal, grayData[i]);
        maxVal = max(maxVal, grayData[i]);
    }
    //Spread all the values to the interval [min;max]
    double delta = maxVal - minVal;
    for(int i = 0; i < width*height; i++)
    {
        grayData[i] = (grayData[i] - minVal) / delta;
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
