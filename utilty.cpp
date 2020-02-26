#include "utilty.h"
#include <QMessageBox>
#include <QFile>

QLibrary* load_library (QString name)
{
    QLibrary *lib = new QLibrary(name);
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
