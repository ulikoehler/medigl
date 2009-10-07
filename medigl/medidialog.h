#ifndef MEDIDIALOG_H
#define MEDIDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MediDialog;
}

class MediDialog : public QDialog {
    Q_OBJECT
public:
    MediDialog(QWidget *parent = 0);
    ~MediDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MediDialog *m_ui;
};

#endif // MEDIDIALOG_H
