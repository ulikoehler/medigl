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
