#include <QApplication>
#include <QImage>
#include <QVector>
#include "vertebradialog.h"
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
    //Construct a new VERTEBRA dialog object without any parent and show it
    VertebraDialog window(0);
    window.show(); //Blocks until the window is closed.
    //After the window is closed, exit the application.
    return app.exec(); //Image memory is free()ed automatically after exit
}
