#include "toolbar.h"
#include <QDebug>
ToolBar::ToolBar(QWidget* parent, bool picW, bool fileW, bool treeW)
    :QToolBar(parent)
{
    setMovable(false);
    QAction *m_newA = new QAction(QIcon(":/resource/new.png"), "Создать проект", this);
    m_newA->setShortcut(QKeySequence(tr("Alt+N")));
    QAction *m_openA = new QAction(QIcon(":/resource/open.png"), "Открыть проект", this);
    m_openA->setShortcut(QKeySequence(tr("Alt+O")));
    m_closeA = new QAction(QIcon(":/resource/close.png"), "Закрыть проект", this);
    m_closeA->setShortcut(QKeySequence(tr("Alt+C")));
    m_saveDg = new QAction(QIcon(":/resource/save.png"), "Сохранить диаграмму", this);
    m_run = new QAction(QIcon(":/resource/diagram.png"), "Построить диаграмму",this);
    m_saveAll = new QAction(QIcon(":/resource/save_all.png"), "Сохранить все диаграммы", this);
    m_analyze = new QAction(QIcon(":/resource/analyze.png"), "Анализировать", this);
    m_description = new QAction(QIcon(":/resource/description.png"),"Открыть описание", this);
    m_report = new QAction(QIcon(":/resource/report.png"),"Генерировать отчет", this);
    QAction *m_fileDW = new QAction(QIcon(":/resource/file.png"), "Показать/скрыть файловый виджет", this);
    m_fileDW->setCheckable(true);
    m_fileDW->setChecked(fileW);
    QAction *m_treeDW = new QAction(QIcon(":/resource/Structure.png"), "Показать/скрыть виджет структуры", this);
    m_treeDW->setCheckable(true);
    m_treeDW->setChecked(treeW);
    QAction *m_pictureDW = new QAction(QIcon(":/resource/picture.png"), "Показать/скрыть виджет изображения", this);
    m_pictureDW->setCheckable(true);
    m_pictureDW->setChecked(picW);

    addActions(QList<QAction*> {m_newA, m_openA, m_closeA});
    addSeparator();
    addActions(QList<QAction*> {m_fileDW, m_treeDW, m_pictureDW});
    addSeparator();
    addActions(QList<QAction*> {m_run, m_saveDg, m_saveAll, m_analyze, m_description, m_report});

    connect(m_newA, SIGNAL(triggered()), SIGNAL(newProject()));
    connect(m_openA, SIGNAL(triggered()), SIGNAL(openProject()));
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
}

void ToolBar::activateActions(bool flag)
{
    m_closeA->setEnabled(flag);
    m_run->setEnabled(flag);
    m_saveDg->setEnabled(flag);
    m_saveAll->setEnabled(flag);
    m_analyze->setEnabled(flag);
    m_description->setEnabled(flag);
    m_report->setEnabled(flag);
}
