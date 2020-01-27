#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QToolBar>

class ToolBar: public  QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr, bool picW=true, bool fileW=true, bool treeW=true);
    void activateActions(bool flag);

signals:
    void newProject();
    void openProject();
    void closeProject();
    void saveDiagram();
    void saveAll();
    void buildDiagram();

    void visibleStructure(bool is_visible);
    void visiblePicture(bool is_visible);
    void visibleFile(bool is_visible);

    void analyze();
    void description();
    void report();

private:
    QAction*m_closeA, *m_saveDg, *m_run, *m_saveAll, *m_analyze, *m_description, *m_report;
    /* *m_newA, *m_openA,*/ /* *m_fileDW,
            *m_treeDW, *m_pictureDW, */
};

#endif // TOOLBAR_H
