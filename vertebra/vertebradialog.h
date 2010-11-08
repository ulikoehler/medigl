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

#ifndef VERTEBRADIALOG_H
#define VERTEBRADIALOG_H

#include <QtGui>
#include <QVector>
#include <memory>
#include "fastimage.h"
#include "dicomimagefile.h"
using namespace std;

namespace Ui {
    class VertebraDialog;
}


/**
 * VERTEBRA main dialog - Contains the GL widget (where the 3D visualisation is shown) and
 * widgets to control the data shown in the GL widget.
 *
 * When VERTEBRA ist started, an instance of this dialog is shown. After closing this dialog, VERTEBRA automatically exits.
 */
class VertebraDialog : public QDialog {
    Q_OBJECT
public:
    /**
     * Constructs a new VERTEBRA dialog.
     */
    VertebraDialog(QWidget *parent);
    ~VertebraDialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *);


private:
    Ui::VertebraDialog *m_ui;

private slots:
    /**
     * The 'Z extent' defines the distance of the individual image layers.
     * This features was added to allow to view the layers separately or alltogether.
     */
    void on_zSpreadSpinBox_valueChanged(double );
    /**
     * 'Reset view' is a feature that resets the scaling, rotation and translation parameters.
     * This slot only calls the resetView() function in the GLWidget class.
     */
    void on_resetViewButton_clicked();
    /**
     * Slot function called when the 'Open DICOM/images' button is clicked.
     * Pops up a file selection dialog to ask the user which files should be opened.
     */
    void on_openButton_clicked();
};

#endif // VERTEBRADIALOG_H
