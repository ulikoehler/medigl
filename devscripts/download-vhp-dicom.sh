#!/bin/sh
#This quick-and-dirty scripts downloads the DICOM files from the Visible Human Project
echo "Downloading Head DICOM data"
mkdir Head
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Head/thumbnails/Head.html
egrep "\.\./.*\.dcm" Head.html -o | cut -c 4- > h.txt
for line in `cat h.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Head/"$line | xargs wget -q -P Head
	done
rm Head.html h.txt
mkdir Ankle
echo "Downloading Ankle DICOM data"
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Ankle/thumbnails/Ankle.html
egrep "\.\./.*\.dcm" Ankle.html -o | cut -c 4- > a.txt
for line in `cat a.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Ankle/"$line | xargs wget -q -P Ankle
	done
rm Ankle.html a.txt