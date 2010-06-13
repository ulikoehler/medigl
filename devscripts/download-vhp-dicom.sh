#!/bin/sh
#This quick-and-dirty scripts downloads the DICOM files from the Visible Human Project
mkdir Male
mkdir Female
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Ankle.tar.gz
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Head.tar.gz
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Hip.tar.gz
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Knee.tar.gz
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Pelvis.tar.gz
wget -P Female http://mri.radiology.uiowa.edu/VHDicom/VHFCT1mm/VHF-Shoulder.tar.gz
wget -P Male http://mri.radiology.uiowa.edu/VHDicom/VHMCT1mm/VHMCT1mm_Head.tar.gz
wget -P Male http://mri.radiology.uiowa.edu/VHDicom/VHMCT1mm/VHMCT1mm_Shoulder.tar.gz
wget -P Male http://mri.radiology.uiowa.edu/VHDicom/VHMCT1mm/VHMCT1mm_Pelvis.tar.gz
wget -P Male http://mri.radiology.uiowa.edu/VHDicom/VHMCT1mm/VHMCT1mm_Hip.tar.gz