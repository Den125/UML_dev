#ifndef PROJECT_DATA_H
#define PROJECT_DATA_H

#include <QString>
#include <QVector>
#include <QSet>
#include <QImage>
#include "diagram.h"

class Project_data
{
public:
    Project_data();
    void sort();
    void clear();
    void load(QVector<Diagram> diagrams);
    bool checkDiagram(QString name);
    void checkDeleted(QVector<Diagram> diagrams);
    void UpdateDiagram(Diagram diag);

    Diagram::Type getTypeDiagram(QString name);
    Diagram getDiagramByName(QString name);
    QImage getImageByName(QString name);
    QStringList getRobustnessList();
    bool deleteDiagram(QString name);

    QVector<Diagram> m_diagrams;
    QSet<QString> m_deleted;
};

#endif // PROJECT_DATA_H
