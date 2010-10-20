#include "vertebradialog.h"
#include "ui_vertebradialog.h"
using namespace std;

VertebraDialog::VertebraDialog(QWidget *parent, vector<FastImage*> images, uint width, uint height) :
    QDialog(parent), m_ui(new Ui::VertebraDialog)
{
    m_ui->setupUi(this);
    m_ui->glWidget->updateImages(images, width, height);
    m_ui->glWidget->setFocus();
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
        cout << "DEBUG: Cancelling image loading" << endl;
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
                                                                      m_ui->contrastExtensionCheckbox->isChecked(),
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
            cout << "All images must have the same width and height!\n";
            exit(1);
        }
        images.push_back(img);
    }
    //CPU-Interpolate images if neccessary
    uint numInterpolationImages = m_ui->interpolateImagesSpinBox->value();
    cout << "DEBUG: Interpolating " << numInterpolationImages << " images between each pair" << endl;
    if(numInterpolationImages == 1) //Interpolate 1 image
    {
        vector<FastImage*>::iterator it =  images.begin();
        vector<FastImage*>::iterator end =  images.end();
        while(true)
        {
            //Get the image and its successor
            FastImage* leftImage = *it;
            it++;
            if(it == end) {
                break;
            }
            FastImage* rightImage = *it;
            //STL vector::insert inserts the image at the position BEFORE the iterator given as argument
            images.insert(it, FastImage::interpolateSingleGrayImage(leftImage, rightImage));
        }
    }
    else if(numInterpolationImages > 1) //Interpolate more than 1 image
    {
        vector<FastImage*>::iterator it =  images.begin();
        vector<FastImage*>::iterator end =  images.end();
        while(true)
        {
            //Get the image and its successor
            FastImage* leftImage = *it;
            it++;
            if(it == end) {
                break;
            }
            FastImage* rightImage = *it;
            //STL vector::insert inserts the image at the position BEFORE the iterator given as argument
            //Here we insert the complete list
            list<FastImage*> interpolatedImages = FastImage::interpolateMultipleGrayImages(leftImage, rightImage, numInterpolationImages);
            images.insert(it, interpolatedImages.begin(), interpolatedImages.end());
        }
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

void VertebraDialog::on_focusGLButton_clicked()
{
    m_ui->glWidget->setFocus();
}
