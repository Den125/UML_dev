#include "treewidget.h"
#include "diagram.h"
#include "project.h"

TreeWidget::TreeWidget(QWidget *parent)
    : QTreeWidget(parent) {
    setHeaderItem(new QTreeWidgetItem(QStringList() << "Structure"));

    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            SLOT(onItemSelected(QTreeWidgetItem*, int)));
}

void TreeWidget::load(QVector<Diagram> project)
{
    m_project = project;

    std::sort(project.begin(), project.end(), [](const Diagram& a, const Diagram& b) {
        if (a.m_type != b.m_type) {
            return int (a.m_type) < int (b.m_type);
        }

        return a.m_name < b.m_name;
    });

    QTreeWidgetItem *chapter = nullptr;

    for (size_t i = 0; i < project.size(); ++i) {
        if (i == 0 || project[i].m_type != project[i-1].m_type) {
            chapter = new QTreeWidgetItem(this, Type::Chapter);
            chapter->setText(0, project_ns::type_to_string(project[i].m_type));
        }
        if (chapter) {
            QTreeWidgetItem *item = new QTreeWidgetItem(chapter, Type::Item);
            item->setText(0, project[i].m_name);
        }
    }
}

void TreeWidget::onItemSelected(QTreeWidgetItem *item, int column)
{
    if (item->type() == Type::Chapter) {
        return;
    }

    auto it = std::find_if(m_project.begin(), m_project.end(), [=](Diagram diagram) {
        return diagram.m_name == item->text(column);
    });

    if (it != m_project.end()) {
        emit selected(*it);
    }
}
