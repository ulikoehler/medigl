#include "dicomimagefile.h"

//Macro for adressing pixels int virtually 2d arrays
//The same as is fastimage.cpp but copied because it maybe needs to be changed
#define REL_ADDR_2D(width, x, y) (x * width + y)

DICOMImageFile::DICOMImageFile(string filename)
{
    image = shared_ptr<DicomImage>(new DicomImage(filename.c_str()));
    if (image != 0)
    {
        if (image->getStatus() == EIS_Normal)
        {
            width = image->getWidth();
            height = image->getHeight();
            image->getFrameCount();

            Uint8 *pixelData = (Uint8 *)(image->getOutputData(8));
            if (pixelData != NULL)
            {
                /* do something useful with the pixel data */
            }
        }
        else
        {
            cerr << "Error: Failed to load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
        }
    }
}


shared_ptr<FastImage> DICOMImageFile::getFastImage(uint frame)
{
    //Check whether the frame number is inside the bounds
    if(frame > frameCount)
    {
        cerr << "Frame not in DICOM image: " << frame << endl;
    }
    //Create a new FastImage instance...
    shared_ptr<FastImage> img(new FastImage(width, height, true));
    int outputSize = image->getOutputDataSize(8);
    char* buffer = (char*) image->getOutputData(8, frame);
    //...and copy the data into it
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; x++)
        {
            img->setPixel(x,y, buffer[REL_ADDR_2D(width, x,y)]);
        }
    }
    return img;
}
