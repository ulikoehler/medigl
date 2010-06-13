#!/bin/sh
#This quick-and-dirty scripts downloads the DICOM files from the Visible Human Project
echo "Downloading Ankle DICOM data"
mkdir Ankle
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Ankle/thumbnails/Ankle.html
egrep "\.\./.*\.dcm" Ankle.html -o | cut -c 4- > Ankle.txt
for line in `cat Ankle.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Ankle/"$line | xargs wget -q -P Ankle
	done
rm Ankle.html Ankle.txt
echo "Downloading Head DICOM data"
mkdir Head
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Head/thumbnails/Head.html -q
egrep "\.\./.*\.dcm" Head.html -o | cut -c 4- > Head.txt
for line in `cat Head.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Head/"$line | xargs wget -q -P Head
	done
rm Head.html Head.txt
mkdir Hip
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Hip/thumbnails/Hip.html -q
egrep "\.\./.*\.dcm" Hip.html -o | cut -c 4- > Hip.txt
for line in `cat Hip.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Hip/"$line | xargs wget -q -P Hip
	done
rm Hip.html Hip.txt
mkdir Knee
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Knee/thumbnails/Knee.html -q
egrep "\.\./.*\.dcm" Knee.html -o | cut -c 4- > Knee.txt
for line in `cat Knee.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Knee/"$line | xargs wget -q -P Knee
	done
rm Knee.html Knee.txt
mkdir Pelvis
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Pelvis/thumbnails/Pelvis.html -q
egrep "\.\./.*\.dcm" Pelvis.html -o | cut -c 4- > Pelvis.txt
for line in `cat Pelvis.txt`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Pelvis/"$line | xargs wget -q -P Pelvis
	done
rm Pelvis.html Pelvis.txt
mkdir Shoulder
wget https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Shoulder/thumbnails/Shoulder.html -q
egrep "\.\./.*\.dcm" Shoulder.html -o | cut -c 4- > Shoulder.txt
for line in `cat Shoulder`
	do
		echo $line
		echo "https://mri.radiology.uiowa.edu//VHDicom/VHFCT1mm/Shoulder/"$line | xargs wget -q -P Pelvis
	done
rm Shoulder.html Shoulder.txt