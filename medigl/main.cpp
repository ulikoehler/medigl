#include <QApplication>

#include "medidialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MediDialog window;
    window.show();
    return app.exec();
}
