#ifndef MEDIDIALOG_H
#define MEDIDIALOG_H

#include <QtGui>
#include <QVector>
#include <memory>
#include "fastimage.h"
#include "dicomimagefile.h"
using namespace std;

namespace Ui {
    class MediDialog;
}

class MediDialog : public QDialog {
    Q_OBJECT
public:
    MediDialog(QWidget *parent, vector<FastImage*> images, uint width, uint height);
    ~MediDialog();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *);


private:
    Ui::MediDialog *m_ui;

private slots:
    void on_textureBlending2DRadioButton_toggled(bool checked);
    void on_linesRadioButton_toggled(bool checked);
    void on_tex3dRadioButton_toggled(bool checked);
    void on_pointCloudRadioButton_toggled(bool checked);
    void on_focusGLButton_clicked();
    void on_zSpreadSpinBox_valueChanged(double );
    void on_resetViewButton_clicked();
    void on_openButton_clicked();
};

#endif // MEDIDIALOG_H
