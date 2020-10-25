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
    //void load(QVector<Diagram> project); //old_load
    void load(); //new_load
    QMap<QString,QStringList> getActors();
    QStringList getRobustnessList();
    void analyze_descirption();
    QString m_name;
signals:
    void selected(Diagram);
    void diagram(QString);
    void update(QVector<Diagram>);
public slots:
    void saveDiagram(Diagram diag);
    void saveProject();
    void analyze();
private slots:
    void onItemSelected(QTreeWidgetItem *item, int column);
    void menuClick(QPoint pos);
    void deleteDiagram();
    void openImage();
private:
    void updateTree(QVector<Diagram>& diagrams);
    void checkDeleted(QVector<Diagram>);
    //QVector<Diagram> m_project; //перенесено в синглтон
    //QSet<QString> deleted; //перенесено в синглтон

};

#endif // TREEWIDGET_H
