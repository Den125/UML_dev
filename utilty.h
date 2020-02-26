#ifndef UTILY_H
#define UTILY_H
#include <QLibrary>
#include <QString>

QLibrary* load_library (QString name);
void check_plantuml();

#endif // UTILY_H
