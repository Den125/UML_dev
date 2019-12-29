#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QMainWindow>
#include <QDockWidget>
#include "treewidget.h"
#include "umltabwidget/umltabwidget.h"
#include "picturewidget.h"
#include "singleton.h"
#include "toolbar.h"

class CentralWidget : public QMainWindow
{
    Q_OBJECT
public:
    CentralWidget(QWidget *parent = nullptr);

public slots:
    void newProject();
    void openProject();

private:
    TreeWidget *m_tree;
    UMLTabWidget *m_tabs;
    PictureWidget *m_picture;
    QDockWidget *m_file, *m_image, *m_structure;
    ToolBar *m_tool;

};

#endif // CENTRAL_WIDGET_H
