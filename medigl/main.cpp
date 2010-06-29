#include <QApplication>
#include <QImage>
#include <QVector>
#include "medidialog.h"
#include <list>
#include <memory>
#include <iostream>
#include <GL/glut.h>
#include <dcmtk/dcmimage/diregist.h>
#include <

using namespace std;

int main(int argc, char *argv[])
{
    //Initialize DCMTK modules
    DJEncoderRegistration::registerCodecs(); // register JPEG codecs
    //Initialize GLUT
    glutInit(&argc, argv);
    //Initialize Qt
    QApplication app(argc, argv);
    int width = -1;
    int height = -1;
    //Open files supplied on the command line
    vector<QImage* > images;
    for(int i = 0; i < (argc - 1); i++)
    {
        QImage* img = new QImage(argv[i + 1]);
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

    MediDialog window(0, images, width, height);

    window.show();
    return app.exec(); //Image memory is free'd automatically after exit
}
