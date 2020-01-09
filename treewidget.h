#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include <QSet>
#include "diagram.h"

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
    enum Type {
        Chapter, Item
    };
public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void load(QVector<Diagram> project);
    QString m_name;
signals:
    void selected(Diagram);
    void diagram(QString);
public slots:
    void saveDiagram(Diagram diag);
private slots:
    void onItemSelected(QTreeWidgetItem *item, int column);
    void menuClick(QPoint pos);
    void deleteDiagram();
    void openImage();
private:
    QVector<Diagram> m_project;

};

#endif // TREEWIDGET_H
