#include "dicomimagefile.h"

DICOMImageFile::DICOMImageFile(string filename)
{
    DicomImage* image = new DicomImage("test.dcm");
    if (image != NULL)
    {
      if (image->getStatus() == EIS_Normal)
      {
        Uint8 *pixelData = (Uint8 *)(image->getOutputData(8 /* bits per sample */));
        if (pixelData != NULL)
        {
          /* do something useful with the pixel data */
        }
      } else
        cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
    }
    delete image;
}
