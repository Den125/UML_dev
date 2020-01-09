#ifndef DESCRIPTIONWIDGET_H
#define DESCRIPTIONWIDGET_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include "structures.h"

class PrecedentWidget;

class DescriptionWidget:public QDialog
{
    Q_OBJECT
public:
    DescriptionWidget(QWidget *parent, QStringList list_precedents, QMap<QString,QStringList> list_actors);
private slots:
    void changePrecedent(QString text);
private:
   void closeEvent(QCloseEvent *event);
   PrecedentWidget *m_widget_seq;
   QMap<QString,QStringList> m_list_actors;
};

#endif // DESCRIPTIONWIDGET_H
