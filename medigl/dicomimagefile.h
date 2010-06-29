#ifndef DICOMIMAGEFILE_H
#define DICOMIMAGEFILE_H

#include "fastimage.h"

#include <string>
#include <tr1/memory>
using namespace std;
using namespace std::tr1;

//DCMTK includes
#include <dcmtk/dcmimgle/dcmimage.h>

/**
 * Wrapper class for easy use of the DICOM file format
 * Uses the DCMTK library to process DICOM files.
 */
class DICOMImageFile
{
public:
    DICOMImageFile(string filename);
    uint getWidth()
    {
        return width;
    }
    uint getHeight()
    {
        return height;
    }
protected:
    shared_ptr<DicomImage> image;
    uint width;
    uint height;
};

#endif // DICOMIMAGEFILE_H
