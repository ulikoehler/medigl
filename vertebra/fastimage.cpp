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
    //#define VERBOSE_DEBUG
#ifdef VERBOSE_DEBUG
    cout << "WxH=" << width * height << " x "<<x << "y" << y << "   addr " << REL_ADDR_2D(width, x, y) << endl;
    cout << qRed(val) << "  " << qGreen(val) << "  " << qBlue(val) << "\n";
#endif
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

FastImage* FastImage::interpolateSingleGrayImage(FastImage *left, FastImage *right)
{
    if(left->getWidth() != right->getWidth() || left->getHeight() != right->getHeight())
    {
        cerr << "interpolateSingleGrayImage(): The extents of the left and right images don't match" << endl;
        return NULL; //This will most probably cause the application to crash but it's acceptable because this application's purpose is proof of concept
    }
    uint width = left->getWidth();
    uint height = left->getHeight();
    FastImage* interpolatedImage = new FastImage(width, height);
    for(uint x = 0; x < width; x++)
    {
        for(uint y = 0; y < height; y++)
        {
            interpolatedImage->setGrayPixel(x,y,(unsigned char)((left->getGray(x,y) + right->getGray(x,y)) / 2)); //Integer division
        }
    }
    return interpolatedImage;
}

list<FastImage*> FastImage::interpolateMultipleGrayImages(FastImage* left, FastImage* right, uint size)
{
    list<FastImage*> images;
    if(left->getWidth() != right->getWidth() || left->getHeight() != right->getHeight())
    {
        cerr << "interpolateMultipleGrayImages(): The extents of the left and right images don't match" << endl;
        return images; //Return an empty list -> nothing is shown on the screen
    }
    for(int s = 0; s < size; s++) //For each image to be interpolated
    {
        //Note that the first image to be interpolated is the image the is the closest one to the image called left
        uint width = left->getWidth();
        uint height = left->getHeight();
        FastImage* interpolatedImage = new FastImage(width, height);
        for(uint x = 0; x < width; x++)
        {
            for(uint y = 0; y < height; y++)
            {
                interpolatedImage->setGrayPixel(x,y,(unsigned char)(left->getGray(x,y) + s * ((double)(right->getGray(x,y) - left->getGray(x,y)) / (double)size)));
            }
        }
        images.push_back(interpolatedImage);
    }
    return images;
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
