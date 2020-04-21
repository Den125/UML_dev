#include "centralwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CentralWidget w;
    w.show();
    w.load();
    return a.exec();
}
