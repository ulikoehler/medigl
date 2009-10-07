#include "medidialog.h"
#include "ui_medidialog.h"

MediDialog::MediDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MediDialog)
{
    m_ui->setupUi(this);
}

MediDialog::~MediDialog()
{
    delete m_ui;
}

void MediDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
