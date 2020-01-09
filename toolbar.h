#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QToolBar>


class ToolBar: public  QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    void activateActions(bool flag);
signals:
    void newProject();
    void openProject();
    void closeProject();
    void saveDiagram();
    void saveAll();
    void buildDiagram();
private:
    QAction *m_newA, *m_openA, *m_closeA, *m_saveDg, *m_run, *m_saveAll;
};

#endif // TOOLBAR_H
