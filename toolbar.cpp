#include "toolbar.h"

ToolBar::ToolBar(QWidget* parent)
    :QToolBar(parent)
{
    setMovable(false);
    m_newA = new QAction("Создать проект", this);
    //m_newA->setShortcut(QKeySequence(Qt::Key_Alt+Qt::Key_N));
    m_openA = new QAction("Открыть проект", this);
   // m_openA->setShortcut(QKeySequence(Qt::Key_Alt,Qt::Key_O));
    m_closeA = new QAction("Закрыть проект", this);
   // m_closeA->setShortcut(Qt::Key_Alt+Qt::Key_C);
    m_saveDg = new QAction("Сохранить диаграмму", this);
    addAction(m_newA);
    addAction(m_openA);
    addAction(m_closeA);
    addSeparator();
    addAction(m_saveDg);
    connect(m_newA, SIGNAL(triggered()), SIGNAL(newProject()));
    connect(m_openA, SIGNAL(triggered()), SIGNAL(openProject()));
    connect(m_closeA, SIGNAL(triggered()), SIGNAL(closeProject()));
    connect(m_saveDg, SIGNAL(triggered()), SIGNAL(saveDiagram()));
}
