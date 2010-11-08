/* VERTEBRA - Volumetric Examiner for Radiological/Tomographical Experimental Basic Realtime Analysis
   Copyright (C) 2010 Uli Koehler

   VERTEBRA is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   VERTEBRA is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with VERTEBRA; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301 USA */

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


FastImage* DICOMImageFile::getFastImage(uint frame, bool enableWindow, double windowCenter, double windowWidth)
{
    //Check whether the frame number is inside the bounds
    if(frame > frameCount)
    {
        cerr << "Frame not in DICOM image: " << frame << endl;
    }
    //Create a new FastImage instance...
    FastImage* img = new FastImage(width, height, true);
    //Set the Hounsfield window if the corresponding option is set
    if(enableWindow)
    {
        image->setWindow(windowCenter, windowWidth);
    }
    //int outputSize = image->getOutputDataSize(32);
    uint32_t* buffer = (uint32_t*) image->getOutputData(32, frame);
    //...and copy the data into it
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            img->setGrayPixel(x,y, buffer[REL_ADDR_2D(width, x,y)]);
        }
    }
    return img;
}
