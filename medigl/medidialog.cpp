#include "medidialog.h"
#include "ui_medidialog.h"
#include <QFileDialog>
#include <iostream>
using namespace std;

MediDialog::MediDialog(QWidget *parent, vector<QImage* > images, uint width, uint height) :
    QDialog(parent),
    m_ui(new Ui::MediDialog)
{
    m_ui->setupUi(this);
    m_ui->widget->updateImages(images, width, height);
}

MediDialog::~MediDialog()
{
    delete m_ui;
}

void MediDialog::changeEvent(QEvent *e)
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

void MediDialog::on_openButton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setModal(true);
    dialog.exec();
    QStringList files = dialog.selectedFiles();
    int width = -1;
    int height = -1;
    vector<QImage*> images;
    for(int i = 0; i < files.size(); i++)
    {
        //Check if the image is a DICOM image
        if(files[i].endsWith(".dcm"))
        {

        }
        //The image is a 'standard' image format (not DICOM), so simply open it as QImage
        QImage* img = new QImage(files[i]);
        //Check if all images have the same width and height
        if(width == -1 && height == -1)
        {
            width = img->width();
            height = img->height();
        }
        if(img->width() != width || img->height() != height)
        {
            cout << "All images must have the same width and height!\n";
            exit(1);
        }
        images.push_back(img);
    }
    m_ui->widget->updateImages(images, width, height);
}
