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
 * Wrapper class for easy use of the DICOM file format (DICOM images only)
 * Uses the DCMTK library to process DICOM files.
 */
class DICOMImageFile
{
public:
    /**
     * Constructs a new DICOMImageFile instance by a given filename
     * Occuring errors are logged to cerr.
     */
    DICOMImageFile(string filename);
    /**
     * Constructs a new FastImage instance with the contents of
     * a specific file this DICOM image file
     * \param frame The number of the frame (beginning with 0) to use
     */
    shared_ptr<FastImage> getFastImage(uint frame);
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
    /**
     * \return The frame count of this image
     */
    uint getFrameCount()
    {
        return frameCount;
    }
protected:
    shared_ptr<DicomImage> image;
    uint width;
    uint height;
    uint frameCount;
};

#endif // DICOMIMAGEFILE_H
