#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QToolBar>


class ToolBar: public  QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
signals:
    void newProject();
    void openProject();
    void closeProject();
    void saveDiagram();
private:
    QAction *m_newA, *m_openA, *m_closeA, *m_saveDg;
};

#endif // TOOLBAR_H
