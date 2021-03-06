#include <QDebug>

#include <QLibrary>
#include <QMenu>
#include <QMessageBox>
#include "treewidget.h"
#include "diagram.h"
#include "project.h"
#include "globaldata.h"
#include "description/description_analyze.h"
#include "utilty.h"

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
   clear();
   checkDeleted(project);
   updateTree(project);
   m_project = project;

    std::sort(project.begin(), project.end(), [](const Diagram& a, const Diagram& b) {
        if (a.m_type != b.m_type) {
            return int (a.m_type) < int (b.m_type);
        }

        return a.m_name < b.m_name;
    });

    QTreeWidgetItem* chapter = nullptr;

    for (int i = 0; i < project.size(); ++i) {
        if (i == 0 || project[i].m_type != project[i-1].m_type) {
            chapter = new QTreeWidgetItem(this, Type::Chapter);
            chapter->setText(0, project_ns::type_to_string(project[i].m_type));
        }
        if (chapter) {
            QTreeWidgetItem *item = new QTreeWidgetItem(chapter, Type::Item);
            item->setText(0, project[i].m_name);
            if (std::find(deleted.begin(), deleted.end(),  project[i].m_name) != deleted.end()) {
               item->setTextColor(0,Qt::red);
            }
            else
            {
                item->setTextColor(0,Qt::black);
                deleted.remove(project[i].m_name);
            }
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
        emit diagram(Singleton<GlobalData>::instance().project_path + '/' +
                     project_ns::type_to_string(it->m_type) + '/' + it->m_name + ".png");
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

    it->m_text = diag.m_text;
    project_ns::save(*it);
}

void TreeWidget::menuClick(QPoint pos)
{
    if (currentItem()->type() == Type::Chapter) {
        return;
    }
    QMenu* menu = new QMenu(this);
    QAction* del = new QAction("Удалить диаграмму", this);
    connect(del, SIGNAL(triggered()),
            this, SLOT(deleteDiagram()));
    QAction* pic = new QAction("Отобразить диаграмму",this);
    connect(pic, SIGNAL(triggered()),
            this, SLOT(openImage()));
    menu->addAction(pic);
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

    auto it = std::find_if(m_project.begin(), m_project.end(), [=](Diagram diagram) {
        return diagram.m_name == name;
    });

    if (it == m_project.end()) {
        return;
    }

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

void TreeWidget::openImage()
{
    auto it = std::find_if(m_project.begin(), m_project.end(), [=](Diagram diagram) {
        return diagram.m_name == currentItem()->text(currentColumn());
    });

    if (it == m_project.end()) {
        return;
    }

    QString path = Singleton<GlobalData>::instance().project_path + '/' +
            project_ns::type_to_string(it->m_type) + '/' + it->m_name + ".png";

    emit diagram(path);
}

void TreeWidget::analyze()
{
    QLibrary* lib =load_library("analyzer");
    if (lib == nullptr)
        return;

    typedef QVector<Diagram> (*analyzer)(QVector<Diagram>);
    analyzer analyze = (analyzer) lib->resolve("analyze");
    if (analyze)
    {
        load(analyze(m_project));
        saveProject();
        emit update(m_project);
    }
    delete lib;
}

QMap<QString,QStringList> TreeWidget::getActors()
{
    QLibrary *lib = load_library("analyzer");
    if (lib == nullptr)
        return QMap<QString,QStringList>();

    typedef QMap<QString,QStringList> (*analyzer)(QVector<Diagram>);
    analyzer get_actors = (analyzer) lib->resolve("get_actors_list");

    QMap<QString,QStringList> map_actors;
    if (get_actors)
    {
        map_actors = get_actors(m_project);
    }
    delete lib;
    return map_actors;
}

QStringList TreeWidget::getRobustnessList()
{
    QStringList list;
    foreach (Diagram diag, m_project)
    {
        if (diag.m_type==Diagram::Type::robustness)
        {
            list.append(diag.m_name);
        }
    }
    return list;

}

void TreeWidget::analyze_descirption()
{
    load(description_analyze::analyze(m_project,
                                      Singleton<GlobalData>::instance().project_path + "/description.json"));
    saveProject();
    emit update(m_project);
}

void TreeWidget::checkDeleted(QVector<Diagram> diagrams)
{
    foreach (Diagram diag,m_project)
    {
        auto it = std::find_if(diagrams.begin(), diagrams.end(), [=](Diagram diagram) {
            return diagram.m_name == diag.m_name;
        });

        if (it == diagrams.end())
        {
            deleted.insert(diag.m_name);
        }
        else
        {
            deleted.remove(diag.m_name);
        }
    }
}

void TreeWidget::updateTree(QVector<Diagram>& diagrams)
{
    for (int i=0;i<m_project.count();i++)
    {
        auto it = std::find_if(diagrams.begin(), diagrams.end(), [=](Diagram diagram) {
            return diagram.m_name == m_project[i].m_name;
        });

        if (it == diagrams.end()) {

            diagrams.push_back(m_project[i]);
        }
    }
}

void TreeWidget::saveProject()
{
    project_ns::rewrite(m_project, Singleton<GlobalData>::instance().project_path + '/' + m_name);
    for (int i=0;i<m_project.count();i++)
    {
        project_ns::save(m_project[i]);
    }
}
