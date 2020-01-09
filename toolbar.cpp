#include "toolbar.h"

ToolBar::ToolBar(QWidget* parent)
    :QToolBar(parent)
{
    setMovable(false);
    m_newA = new QAction(QIcon(":/resource/new.png"),"Создать проект", this);
    m_newA->setShortcut(QKeySequence(tr("Alt+N")));
    m_openA = new QAction(QIcon(":/resource/open.png"),"Открыть проект", this);
    m_openA->setShortcut(QKeySequence(tr("Alt+O")));
    m_closeA = new QAction(QIcon(":/resource/close.png"),"Закрыть проект", this);
    m_closeA->setShortcut(QKeySequence(tr("Alt+C")));
    m_saveDg = new QAction(QIcon(":/resource/save.png"),"Сохранить диаграмму", this);
    m_run = new QAction("Построить диаграмму",this);
    m_saveAll = new QAction(QIcon(":/resource/save_all.png"),"Сохранить все диаграммы", this);
    addAction(m_newA);
    addAction(m_openA);
    addAction(m_closeA);
    addSeparator();
    addAction(m_run);
    addAction(m_saveDg);
    addAction(m_saveAll);
    connect(m_newA, SIGNAL(triggered()), SIGNAL(newProject()));
    connect(m_openA, SIGNAL(triggered()), SIGNAL(openProject()));
    connect(m_closeA, SIGNAL(triggered()), SIGNAL(closeProject()));
    connect(m_run, SIGNAL(triggered()), SIGNAL(buildDiagram()));
    connect(m_saveDg, SIGNAL(triggered()), SIGNAL(saveDiagram()));
    connect(m_saveAll, SIGNAL(triggered()), SIGNAL(saveAll()));
}

void ToolBar::activateActions(bool flag)
{
    m_closeA->setEnabled(flag);
    m_run->setEnabled(flag);
    m_saveDg->setEnabled(flag);
    m_saveAll->setEnabled(flag);
}

