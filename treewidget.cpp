#include <QDebug>

#include <QMenu>
#include <QMessageBox>
#include "treewidget.h"
#include "diagram.h"
#include "project.h"
#include "globaldata.h"

TreeWidget::TreeWidget(QWidget* parent)
    : QTreeWidget(parent) {
    setHeaderItem(new QTreeWidgetItem(QStringList() << "Structure"));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(menuClick(QPoint)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            SLOT(onItemSelected(QTreeWidgetItem*, int)));
}

void TreeWidget::load(QVector<Diagram> project)
{
    m_project = project;
   // m_project.fromList(project.toList());
    std::sort(project.begin(), project.end(), [](const Diagram& a, const Diagram& b) {
        if (a.m_type != b.m_type) {
            return int (a.m_type) < int (b.m_type);
        }

        return a.m_name < b.m_name;
    });

    QTreeWidgetItem* chapter = nullptr;

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

void TreeWidget::saveDiagram(Diagram diag)
{
    auto it = std::find_if(m_project.begin(), m_project.end(), [=](Diagram diagram) {
        return diagram.m_name == diag.m_name;
    });

    if (it == m_project.end()) {
        return;
    }

    it->m_text=diag.m_text;
    project_ns::save(*it);
}
void TreeWidget::menuClick(QPoint pos)
{
    if (currentItem()->type() == Type::Chapter) {
        return;
    }
    QMenu *menu = new QMenu(this);
    QAction *del = new QAction("Удалить диаграмму", this);
    connect(del, SIGNAL(triggered()),
            this, SLOT(deleteDiagram()));
    menu->addAction(del);
    menu->popup(viewport()->mapToGlobal(pos));
}

void TreeWidget::deleteDiagram()
{
    QString name = currentItem()->text(currentColumn());
    QMessageBox unsaved(QMessageBox::Question, "Удаление диаграммы",
                        "Вы действительно хотите удалить диаграмму " + name + "?",
                        (QMessageBox::Yes | QMessageBox::No), this);
    if (unsaved.exec() != QMessageBox::Yes)
        return;

   /* auto it = std::find_if(m_project.begin(), m_project.end(), [=](Diagram diagram) {
        return diagram.m_name == name;
    });

    if (it == m_project.end()) {
        return;
    }

   // m_project.removeOne(*it);

    std::remove(m_project.begin(), m_project.end(),it);*/


    //может использовать QMap вместо QVector
    for (int i = 0; i < m_project.count(); i++)
    {
        if (m_project[i].m_name == name)
        {
            m_project.remove(i);
            break;
        }
    }
    delete currentItem();
    project_ns::rewrite(m_project, Singleton<GlobalData>::instance().project_path + '/' + m_name);
}
