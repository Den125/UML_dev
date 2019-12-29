#ifndef PROJECT_H
#define PROJECT_H

#include <QVector>
#include "diagram.h"

namespace project_ns
{
    QVector<Diagram> read(QString filename);
    void save(Diagram diag);
    QString type_to_string(Diagram::Type type);
    void rewrite(QVector<Diagram> vector, QString file_name);
    bool create(QString path, QString name);
};

#endif // PROJECT_H
