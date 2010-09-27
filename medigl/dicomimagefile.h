#ifndef DICOMIMAGEFILE_H
#define DICOMIMAGEFILE_H

#include "fastimage.h"

#include <string>
#include <tr1/memory>
using namespace std;
using namespace std::tr1;

//DCMTK includes
#define HAVE_CONFIG_H
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
     * \param contrastExtension Whether to spread the values in the resulting grayscale image to the full available range. Not recommendened when using a Hounsfield window
     * \param enableWindow Whether to use a Hounsfield window
     * \param windowCenter The center of the Hounsfield window, if enabled
     * \param windowWidth The width of the Hounsfield window, if enabled
     */
    FastImage* getFastImage(uint frame, bool contrastExtension, bool enableWindow = false, double windowCenter = 0.0, double windowWidth = 0.0);
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
    /**
     * Releases all resources acquired by this DICOMImageFile instance
     */
    ~DICOMImageFile();
protected:
    DicomImage* image;
    uint width;
    uint height;
    uint frameCount;
};

#endif // DICOMIMAGEFILE_H
