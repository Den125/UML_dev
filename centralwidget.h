#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include "treewidget.h"
#include "umltabwidget/umltabwidget.h"
#include "singleton.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(QWidget *parent = nullptr);

private:
    TreeWidget* m_tree;
    UMLTabWidget* m_tabs;

    QPushButton *m_open, *m_save_dg;
};

#endif // CENTRAL_WIDGET_H
