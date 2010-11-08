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

#include "vertebradialog.h"
#include "ui_vertebradialog.h"
using namespace std;

VertebraDialog::VertebraDialog(QWidget *parent) :
    QDialog(parent), m_ui(new Ui::VertebraDialog)
{
    m_ui->setupUi(this);
    m_ui->glWidget->setFocus(); //To allow keystrokes
}

VertebraDialog::~VertebraDialog()
{
    delete m_ui;
}

void VertebraDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void VertebraDialog::keyPressEvent(QKeyEvent *event)
{
    m_ui->glWidget->keyPressEvent(event);
    event->ignore();
}

void VertebraDialog::on_openButton_clicked()
{
    //Let the user choose if he wants to
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setModal(true);
    if(!dialog.exec()) {
        //The user didn't click "Open" but cancelled the operation
        return;
    }
    //If the next lines are executed, the user clicked open.
    QStringList files = dialog.selectedFiles();
    int width = -1;
    int height = -1;
    vector<FastImage*> images;
    for(int i = 0; i < files.size(); i++)
    {
        //Check if the image is a DICOM image
        FastImage* img;
        if(files[i].endsWith(".dcm"))
        {
            img = DICOMImageFile(files[i].toStdString()).getFastImage(0, //Only use the first image when the DICOM files is a stack
                                                                      m_ui->enableWindowCheckBox->isChecked(),
                                                                      m_ui->windowCenterSpinBox->value(),
                                                                      m_ui->windowWidthSpinBox->value());
        }
        //The image is non-DICOM, so simply open it as QImage. QImage can open everything most image file formats.
        else
        {
            img = new FastImage(new QImage(files[i]));
        }
        //Check if all images have the same width and height
        if(width == -1 && height == -1)
        {
            width = img->getWidth();
            height = img->getHeight();
        }
        if(img->getWidth() != width || img->getHeight() != height)
        {
            cerr << "All images must have the same width and height!\n";
            exit(1);
        }
        images.push_back(img);
    }
    m_ui->glWidget->updateImages(images, width, height);
    m_ui->glWidget->setFocus();
}

void VertebraDialog::on_resetViewButton_clicked()
{
    m_ui->glWidget->resetView();
    m_ui->glWidget->setFocus();
}

void VertebraDialog::on_zSpreadSpinBox_valueChanged(double val)
{
    m_ui->glWidget->setZExtent(val);
}
