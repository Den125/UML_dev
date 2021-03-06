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
    void activateControls(bool picW=true, bool fileW=true, bool treeW=true);
signals:
    void newProject();
    void openProject();
    void saveProject();
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

    void about();
private:
    QAction *m_saveProject, *m_closeA, *m_saveDg, *m_run,
    *m_saveAll, *m_analyze, *m_description, *m_report, *m_pictureDW, *m_treeDW, *m_fileDW;
};

#endif // TOOLBAR_H
