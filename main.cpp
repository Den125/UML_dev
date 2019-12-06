#include "centralwidget.h"

#include "singleton.h"
#include <QApplication>

// сохранение табов
// в функции закрытия таба проверить наличие несохраненных изменений, выдать окно ...

// ... удаление диаграммы в treewidget

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CentralWidget w;
    w.show();

    return a.exec();
}
