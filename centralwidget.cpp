#include "centralwidget.h"
#include <QGridLayout>
#include <QDebug>
#include "project.h"
#include <QFileDialog>

#include "globaldata.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent),
      m_tree(new TreeWidget(this)),
      m_tabs(new UMLTabWidget(this)),
      m_open(new QPushButton("&Open project", this)),
      m_save_dg(new QPushButton("&Save Diagram", this)){

    auto layer = new QGridLayout(this);

    layer->addWidget(m_tree, 0, 0);
    layer->addWidget(m_tabs, 0, 1);
    layer->addWidget(m_open, 1, 0);
    layer->addWidget(m_save_dg, 1, 1);

    connect(m_tree, SIGNAL(selected(Diagram)), m_tabs, SLOT(createTab(Diagram)));

    connect(m_tabs, SIGNAL(save(Diagram)), m_tree, SLOT(saveDiagram(Diagram)));

    connect(m_open, &QPushButton::clicked, [this] {
        QString path = QFileDialog::getOpenFileName(nullptr, "Выбор файла проекта", "", "UMLParserProject (*.upp)");
        if (path.isNull())
            return;

        m_tree->load(project_ns::read(path));

        Singleton<GlobalData>::instance().project_path = path.left(path.lastIndexOf('/'));

        m_tree->m_name = path.remove(0, path.lastIndexOf('/') + 1);

    });

    connect(m_save_dg, SIGNAL(clicked()), m_tabs, SLOT(saveTab()));
}

