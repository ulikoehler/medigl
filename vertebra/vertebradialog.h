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
