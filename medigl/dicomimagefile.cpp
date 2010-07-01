#include "dicomimagefile.h"

//Macro for adressing pixels int virtually 2d arrays
//The same as is fastimage.cpp but copied because it maybe needs to be changed
#define REL_ADDR_2D(width, x, y) (x * width + y)

DICOMImageFile::DICOMImageFile(string filename)
{
    image = new DicomImage(filename.c_str());
    if (image != 0)
    {
        if (image->getStatus() == EIS_Normal)
        {
            width = image->getWidth();
            height = image->getHeight();
        }
        else
        {
            cerr << "Error: Failed to load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
        }
    }
}

DICOMImageFile::~DICOMImageFile()
{
    delete image;
}


FastImage* DICOMImageFile::getFastImage(uint frame)
{
    //Check whether the frame number is inside the bounds
    if(frame > frameCount)
    {
        cerr << "Frame not in DICOM image: " << frame << endl;
    }
    //Create a new FastImage instance...
    FastImage* img = new FastImage(width, height, true);
    int outputSize = image->getOutputDataSize(32);
    uint32_t* buffer = (uint32_t*) image->getOutputData(32, frame);
    //...and copy the data into it
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            img->setGrayPixel(x,y, buffer[REL_ADDR_2D(width, x,y)]);
        }
    }
    img->spreadContrast(); //Increase the contrast to make the data visible
    return img;
}
