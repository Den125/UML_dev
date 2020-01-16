#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QMainWindow>

class QDockWidget;
class TreeWidget;
class UMLTabWidget;
class PictureWidget;
class Running;
class ToolBar;
#include "singleton.h"


class CentralWidget : public QMainWindow
{
    Q_OBJECT
public:
    CentralWidget(QWidget *parent = nullptr);

public slots:
    void newProject();
    void openProject();
    void closeProject();
    void building();
    void showOrHide(int);
    void endBuilding();
private slots:
    void closeEvent(QCloseEvent* event);

private:
    void open_ini();
    void createWorkspace();

    TreeWidget *m_tree;
    UMLTabWidget *m_tabs;
    PictureWidget *m_picture;
    QDockWidget *m_file, *m_image, *m_structure;
    ToolBar *m_tool;
    Running *m_run;
};

#endif // CENTRAL_WIDGET_H
