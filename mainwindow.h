#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treewidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void elem(Diagram diag);
private:
    TreeWidget* m_tree;
    QString m_project_path;
};
#endif // MAINWINDOW_H
