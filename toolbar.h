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
    void visible(int ind);
    void analyze();
private slots:
    void showOrHidePicture();
    void showOrHideStructure();
    void showOrHideFile();

private:
    QAction *m_newA, *m_openA, *m_closeA, *m_saveDg, *m_run, *m_saveAll, *m_fileDW,
            *m_treeDW, *m_pictureDW, *m_analyze;
};

#endif // TOOLBAR_H
