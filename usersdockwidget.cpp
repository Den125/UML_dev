#include "usersdockwidget.h"
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QDebug>

UsersDockWidget::UsersDockWidget(QMainWindow* parent, QString name)
    : QDockWidget(name,parent),
      m_parent(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(dockWidget_customContextMenuRequested(QPoint)));
}

void UsersDockWidget::dockWidget_customContextMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *right = new QAction("Переместить на правый виджет", this);
    QAction *left = new QAction("Переместить на левый виджет", this);
    QAction *up = new QAction("Переместить на верхний виджет", this);
    QAction *down = new QAction("Переместить на нижний виджет", this);
    connect(right, SIGNAL(triggered()), this, SLOT(move_right()));
    connect(left, SIGNAL(triggered()), this, SLOT(move_left()));
    connect(up, SIGNAL(triggered()), this, SLOT(move_up()));
    connect(down,SIGNAL(triggered()),this,SLOT(move_down()));
    menu->addAction(right);
    menu->addAction(left);
    menu->addAction(up);
    menu->addAction(down);
    menu->popup(mapToGlobal(pos));
}

void UsersDockWidget::move_up()
{
    m_parent->addDockWidget(Qt::TopDockWidgetArea,this);
}

void UsersDockWidget::move_down()
{
    m_parent->addDockWidget(Qt::BottomDockWidgetArea,this);
}

void UsersDockWidget::move_left()
{
    m_parent->addDockWidget(Qt::LeftDockWidgetArea,this);
}

void UsersDockWidget::move_right()
{
    m_parent->addDockWidget(Qt::RightDockWidgetArea,this);
}
