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

class VertebraDialog : public QDialog {
    Q_OBJECT
public:
    VertebraDialog(QWidget *parent, vector<FastImage*> images, uint width, uint height);
    ~VertebraDialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *);


private:
    Ui::VertebraDialog *m_ui;

private slots:
    void on_focusGLButton_clicked();
    void on_zSpreadSpinBox_valueChanged(double );
    void on_resetViewButton_clicked();
    void on_openButton_clicked();
};

#endif // VERTEBRADIALOG_H
