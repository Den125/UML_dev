#include "utilty.h"
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QDir>

QLibrary* load_library (QString name)
{
    #ifdef Q_OS_WIN
    QLibrary *lib = new QLibrary(QCoreApplication::applicationDirPath()+QDir::separator()+name);
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
    QFile plantuml(QCoreApplication::applicationDirPath()+QDir::separator()+"plantuml.jar");
    qDebug()<<QCoreApplication::applicationDirPath()<<QFile(QCoreApplication::applicationDirPath()+QDir::separator()+"plantuml.jar").exists();
    if (!plantuml.exists())
    {
        QMessageBox error(QMessageBox::Critical, "Ошибка!",
                          "Не найден файл plantuml.jar!"
                          "Построение диаграмм может не работать!",
                          QMessageBox::Ok, nullptr);
        error.exec();
    }
}
