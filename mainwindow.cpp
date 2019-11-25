#include "mainwindow.h"
#include <QGridLayout>
#include <QDebug>
#include "project.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), m_tree(new TreeWidget(this)) {

    auto layer = new QGridLayout(this);
    setLayout(layer);

    m_tree->load(project_ns::read("project.upp"));

    layer->addWidget(m_tree, 0, 0);

    connect(m_tree, SIGNAL(selected(Diagram)), this, SLOT(elem(Diagram)));
}

void MainWindow::elem(Diagram diag)
{
    qDebug() << diag.m_name;
}

