#ifndef SEQUENCEWIDGET_H
#define SEQUENCEWIDGET_H

#include <QObject>
#include <QWidget>
#include "structures.h"

class PrecedentWidget:public QWidget
{
    Q_OBJECT
public:
    PrecedentWidget(QWidget *parent, QString text, QStringList list_actors);
    void savePrecedent();
private slots:
    void add_seq();
    void delete_seq();
    void add_punkt();
    void delete_punkt();
    void set_into_table();
    void setUsl(QString usl);
    void setGoal(QString goal);
    void setFace(QString goal);
private:
    Structures::precedentData m_data;
    QString m_name;
};

#endif // SEQUENCEWIDGET_H
