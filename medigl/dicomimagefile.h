#ifndef DICOMIMAGEFILE_H
#define DICOMIMAGEFILE_H

#include "fastimage.h"

#include <string>
#include <tr1/memory>
using namespace std;

/**
 * Wrapper class for easy use of the DICOM file format
 * Uses the DCMTK library to process DICOM files.
 */
class DICOMImageFile
{
public:
    DICOMImageFile(string filename);
protected:
    shared_pt
};

#endif // DICOMIMAGEFILE_H
