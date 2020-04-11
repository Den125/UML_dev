#include "utilty.h"
#include <QMessageBox>
#include <QFile>
#include <QApplication>

QLibrary* load_library (QString name)
{
    #ifdef Q_OS_WIN
    QLibrary *lib = new QLibrary(name);
#else
    QLibrary *lib = new QLibrary(QApplication::applicationDirPath()+'/'+name);
#endif
    if (!lib->load())
    {
        QMessageBox error(QMessageBox::Critical, "Ошибка!",
                          "Библиотека " + name + " не обнаружена!"
                          "Пожалуйста проверьте ее наличие!",
                          QMessageBox::Ok, nullptr);
        error.exec();
        return nullptr;
    }
    return lib;
}

void check_plantuml()
{
    QFile plantuml("plantuml.jar");
    if (!plantuml.exists())
    {
        QMessageBox error(QMessageBox::Critical, "Ошибка!",
                          "Не найден файл plantuml.jar!"
                          "Построение диаграмм может не работать!",
                          QMessageBox::Ok, nullptr);
        error.exec();
    }
}
