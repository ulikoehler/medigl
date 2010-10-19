#include <QApplication>
#include <QImage>
#include <QVector>
#include "medidialog.h"
#include <list>
#include <memory>
#include <iostream>
#include <GL/glut.h>
#include "fastimage.h"
using namespace std;

int main(int argc, char *argv[])
{
    //Initialize GLUT
    glutInit(&argc, argv);
    //Initialize Qt
    QApplication app(argc, argv);
    uint width = -1;
    uint height = -1;
    //Open files supplied on the command line
    vector<FastImage* > images;
    for(int i = 0; i < (argc - 1); i++)
    {
        FastImage* img = new FastImage(new QImage(argv[i + 1]));
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

    MediDialog window(0, images, width, height);

    window.show();
    return app.exec(); //Image memory is free'd automatically after exit
}
