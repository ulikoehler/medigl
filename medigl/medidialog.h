#ifndef MEDIDIALOG_H
#define MEDIDIALOG_H

#include <QtGui/QDialog>
#include <QVector>
#include <memory>
#include "fastimage.h"
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


private:
    Ui::MediDialog *m_ui;

private slots:
    void on_openButton_clicked();
};

#endif // MEDIDIALOG_H
