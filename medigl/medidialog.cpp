#include "medidialog.h"
#include "ui_medidialog.h"
using namespace std;

MediDialog::MediDialog(QWidget *parent, vector<FastImage*> images, uint width, uint height) :
    QDialog(parent),
    m_ui(new Ui::MediDialog)
{
    m_ui->setupUi(this);
    m_ui->glWidget->updateImages(images, width, height);
    m_ui->glWidget->setFocus();
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
void MediDialog::keyPressEvent(QKeyEvent *event)
{
    m_ui->glWidget->keyPressEvent(event);
    event->ignore();
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
    vector<FastImage*> images;
    for(int i = 0; i < files.size(); i++)
    {
        //Check if the image is a DICOM image
        FastImage* img;
        if(files[i].endsWith(".dcm"))
        {
            img = DICOMImageFile(files[i].toStdString()).getFastImage(0);
        }
        //The image is a 'standard' image format (not DICOM), so simply open it as QImage
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
    m_ui->glWidget->updateImages(images, width, height);
    m_ui->glWidget->setFocus();
}

void MediDialog::on_resetViewButton_clicked()
{
    m_ui->glWidget->resetView();
    m_ui->glWidget->setFocus();
}

void MediDialog::on_zSpreadSpinBox_valueChanged(double val)
{
    m_ui->glWidget->setZExtent(val);
}

void MediDialog::on_focusGLButton_clicked()
{
    m_ui->glWidget->setFocus();
}

void MediDialog::on_pointCloudRadioButton_toggled(bool checked)
{
    if(checked)
    {
        m_ui->glWidget->setRenderingMethod(PointCloud); //Automatically updates the screen
        cout << "Setting point cloud rendering method" << endl;
    }
}

void MediDialog::on_tex3dRadioButton_toggled(bool checked)
{
    if(checked)
    {
        m_ui->glWidget->setRenderingMethod(Texture3D); //Automatically updates the screen
        cout << "Setting texture 3D rendering rendering method" << endl;
    }
}

void MediDialog::on_linesRadioButton_toggled(bool checked)
{
    if(checked)
    {
        m_ui->glWidget->setRenderingMethod(Lines); //Automatically updates the screen
        cout << "Setting lines rendering method" << endl;
    }
}

void MediDialog::on_textureBlending2DRadioButton_toggled(bool checked)
{
    if(checked)
    {
        m_ui->glWidget->setRenderingMethod(TextureBlending2D); //Automatically updates the screen
        cout << "Setting texture 2D blending rendering method" << endl;
    }
}
