#ifndef PROJECT_H
#define PROJECT_H

#include <QVector>
#include "diagram.h"

namespace project_ns
{
    QVector<Diagram> read(QString filename);
    QString type_to_string(Diagram::Type type);
};

#endif // PROJECT_H
