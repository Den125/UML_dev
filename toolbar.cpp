#include "toolbar.h"
#include <QDebug>
ToolBar::ToolBar(QWidget* parent)
    :QToolBar(parent)
{
    setMovable(false);
    QAction *m_newA = new QAction(QIcon(":/resource/new.png"), "Создать проект (Ctrl+N)", this);
    m_newA->setShortcut(QKeySequence(tr("Ctrl+N")));
    QAction *m_openA = new QAction(QIcon(":/resource/open.png"), "Открыть проект (Ctrl+O)", this);
    m_openA->setShortcut(QKeySequence(tr("Ctrl+O")));
    m_saveProject = new QAction(QIcon(":/resource/saveProject.png"),"Сохранить проект (Ctrl+Shift+S)", this);
    m_saveProject->setShortcut(QKeySequence(tr("Ctrl+Shift+S")));
    m_closeA = new QAction(QIcon(":/resource/close.png"), "Закрыть проект (Ctrl+Q)", this);
    m_closeA->setShortcut(QKeySequence(tr("Ctrl+Q")));
    m_saveDg = new QAction(QIcon(":/resource/save.png"), "Сохранить диаграмму (Ctrl+S)", this);
    m_saveDg->setShortcut(QKeySequence(tr("Ctrl+S")));
    m_run = new QAction(QIcon(":/resource/diagram.png"), "Построить диаграмму (Alt+B)",this);
    m_run->setShortcut(QKeySequence(tr("Alt+B")));
    m_saveAll = new QAction(QIcon(":/resource/save_all.png"), "Сохранить все открытые диаграммы (Alt+F)", this);
    m_saveAll->setShortcut(QKeySequence(tr("Alt+F")));
    m_analyze = new QAction(QIcon(":/resource/analyze.png"), "Анализировать (Alt+A)", this);
    m_analyze->setShortcut(QKeySequence(tr("Alt+A")));
    m_description = new QAction(QIcon(":/resource/description.png"),"Открыть описание (Alt+D)", this);
    m_analyze->setShortcut(QKeySequence(tr("Alt+D")));
    m_report = new QAction(QIcon(":/resource/report.png"),"Генерировать отчет (Ctrl+G)", this);
    m_report->setShortcut(QKeySequence(tr("Ctrl+G")));
    m_fileDW = new QAction(QIcon(":/resource/file.png"), "Показать/скрыть файловый виджет", this);
    m_treeDW = new QAction(QIcon(":/resource/Structure.png"), "Показать/скрыть виджет структуры", this);
    m_pictureDW = new QAction(QIcon(":/resource/picture.png"), "Показать/скрыть виджет изображения", this);
    QAction *m_about = new QAction(QIcon(":/resource/about.png"), "О программе", this);

    addActions(QList<QAction*> {m_newA, m_openA, m_saveProject, m_closeA});
    addSeparator();
    addActions(QList<QAction*> {m_fileDW, m_treeDW, m_pictureDW});
    addSeparator();
    addActions(QList<QAction*> {m_run, m_saveDg, m_saveAll, m_analyze, m_description, m_report});
    addSeparator();
    addAction(m_about);

    connect(m_newA, SIGNAL(triggered()), SIGNAL(newProject()));
    connect(m_openA, SIGNAL(triggered()), SIGNAL(openProject()));
    connect(m_saveProject, SIGNAL(triggered()), SIGNAL(saveProject()));
    connect(m_closeA, SIGNAL(triggered()), SIGNAL(closeProject()));
    connect(m_run, SIGNAL(triggered()), SIGNAL(buildDiagram()));
    connect(m_saveDg, SIGNAL(triggered()), SIGNAL(saveDiagram()));
    connect(m_saveAll, SIGNAL(triggered()), SIGNAL(saveAll()));
    connect(m_fileDW, SIGNAL(toggled(bool)), SIGNAL(visibleFile(bool)));
    connect(m_treeDW, SIGNAL(toggled(bool)), SIGNAL(visibleStructure(bool)));
    connect(m_pictureDW, SIGNAL(toggled(bool)), SIGNAL(visiblePicture(bool)));
    connect(m_analyze, SIGNAL(triggered()), SIGNAL(analyze()));
    connect(m_description, SIGNAL(triggered()), SIGNAL(description()));
    connect(m_report, SIGNAL(triggered()), SIGNAL(report()));
    connect(m_about, SIGNAL(triggered()), SIGNAL(about()));
}

void ToolBar::activateActions(bool flag)
{
    m_saveProject->setEnabled(flag);
    m_closeA->setEnabled(flag);
    m_run->setEnabled(flag);
    m_saveDg->setEnabled(flag);
    m_saveAll->setEnabled(flag);
    m_analyze->setEnabled(flag);
    m_description->setEnabled(flag);
    m_report->setEnabled(flag);
}

void ToolBar::activateControls(bool picW, bool fileW, bool treeW)
{
    m_fileDW->setCheckable(true);
    m_fileDW->setChecked(fileW);
    m_treeDW->setCheckable(true);
    m_treeDW->setChecked(treeW);
    m_pictureDW->setCheckable(true);
    m_pictureDW->setChecked(picW);
}
