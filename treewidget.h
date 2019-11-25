#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
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
signals:
    void selected(Diagram);
private slots:
    void onItemSelected(QTreeWidgetItem *item, int column); //TODO: ...
private:
    QVector<Diagram> m_project;
};

#endif // TREEWIDGET_H
