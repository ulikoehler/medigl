#include "dicomimagefile.h"

DICOMImageFile::DICOMImageFile(string filename)
{
    image = shared_ptr<DicomImage>(new DicomImage(filename.c_str()));
    if (image != 0)
    {
        if (image->getStatus() == EIS_Normal)
        {
            width = image->getWidth();
            height = image->getHeight();
            image->getOutputPlane()
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
    delete image;
}
