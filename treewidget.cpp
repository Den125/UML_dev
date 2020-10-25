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

//Не думаю, что нужно делать load c вектором в качестве параметра. Просто теперь этот вектор допступен в сигнлтоне.
//хотя внутренняя часть пока еще требует доработки, в особенности функция updateTree.
//void TreeWidget::load(QVector<Diagram> project)
void TreeWidget::load()
{
  // QVector<Diagram> project=Singleton<GlobalData>::instance().data.m_diagrams;
   clear();
   //Singleton<GlobalData>::instance().data.checkDeleted(project);
   //updateTree(project);
  // m_project = project;

    /*std::sort(project.begin(), project.end(), [](const Diagram& a, const Diagram& b) {
        if (a.m_type != b.m_type) {
            return int (a.m_type) < int (b.m_type);
        }

        return a.m_name < b.m_name;
    });*/
    Singleton<GlobalData>::instance().data.sort();
    QTreeWidgetItem* chapter = nullptr;

    for (int i = 0; i < Singleton<GlobalData>::instance().data.m_diagrams.size(); ++i) {
        if (i == 0 || Singleton<GlobalData>::instance().data.m_diagrams[i].m_type != Singleton<GlobalData>::instance().data.m_diagrams[i-1].m_type) {
            chapter = new QTreeWidgetItem(this, Type::Chapter);
            chapter->setText(0, project_ns::type_to_string(Singleton<GlobalData>::instance().data.m_diagrams[i].m_type));
        }
        if (chapter) {
            QTreeWidgetItem *item = new QTreeWidgetItem(chapter, Type::Item);
            item->setText(0, Singleton<GlobalData>::instance().data.m_diagrams[i].m_name);
            if (std::find(Singleton<GlobalData>::instance().data.m_deleted.begin(), Singleton<GlobalData>::instance().data.m_deleted.end(),  Singleton<GlobalData>::instance().data.m_diagrams[i].m_name) != Singleton<GlobalData>::instance().data.m_deleted.end()) {
               item->setTextColor(0,Qt::red);
            }
            else
            {
                item->setTextColor(0,Qt::black);
                Singleton<GlobalData>::instance().data.m_deleted.remove(Singleton<GlobalData>::instance().data.m_diagrams[i].m_name);
            }
        }
    }
}

void TreeWidget::onItemSelected(QTreeWidgetItem *item, int column)
{
    if (item->type() == Type::Chapter) {
        return;
    }

    auto it = std::find_if(Singleton<GlobalData>::instance().data.m_diagrams.begin(), Singleton<GlobalData>::instance().data.m_diagrams.end(), [=](Diagram diagram) {
        return diagram.m_name == item->text(column);
    });

    if (it != Singleton<GlobalData>::instance().data.m_diagrams.end()) {
        emit selected(*it);
        emit diagram(Singleton<GlobalData>::instance().project_path + '/' +
                     project_ns::type_to_string(it->m_type) + '/' + it->m_name + ".png");
    }
}

//saveDiagram его тоже по хорошему надо кинуть в project_data, но тут есть привязка к project...
//UPD: project же пространство имен..значит вполне можно..
void TreeWidget::saveDiagram(Diagram diag)
{
    /*
    auto it = std::find_if(Singleton<GlobalData>::instance().project_data.begin(), Singleton<GlobalData>::instance().project_data.end(), [=](Diagram diagram) {
        return diagram.m_name == diag.m_name;
    });

    if (it == Singleton<GlobalData>::instance().project_data.end()) {
        return;
    }
    */
    if (!Singleton<GlobalData>::instance().data.checkDiagram(diag.m_name)) return;

    auto it=Singleton<GlobalData>::instance().data.getDiagramByName(diag.m_name);

    it.m_text = diag.m_text;
    project_ns::save(it);
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

//тут ничего особенного эта часть кода переехала в класс данных проекта
/*    auto it = std::find_if(Singleton<GlobalData>::instance().project_data.begin(), Singleton<GlobalData>::instance().project_data.end(), [=](Diagram diagram) {
        return diagram.m_name == name;
    });

    if (it == Singleton<GlobalData>::instance().project_data.end()) {
        return;
    }

    for (int i = 0; i < Singleton<GlobalData>::instance().project_data.count(); i++)
    {
        if (Singleton<GlobalData>::instance().project_data[i].m_name == name)
        {
            Singleton<GlobalData>::instance().project_data.remove(i);
            break;
        }
    }*/

    if (Singleton<GlobalData>::instance().data.deleteDiagram(name))
        delete currentItem();
    project_ns::rewrite(Singleton<GlobalData>::instance().data.m_diagrams, Singleton<GlobalData>::instance().project_path + '/' + m_name);
}

//тоже кинуть бы в project_data, но опять же привязка к пути проекта...
void TreeWidget::openImage()
{
   // Singleton<GlobalData>::instance().data.getImageByName(currentItem()->text(currentColumn()));
  /*  auto it = std::find_if(Singleton<GlobalData>::instance().project_data.begin(), Singleton<GlobalData>::instance().project_data.end(), [=](Diagram diagram) {
        return diagram.m_name == currentItem()->text(currentColumn());
    });

    if (it == Singleton<GlobalData>::instance().project_data.end()) {
        return;
    }
*/
    if (!Singleton<GlobalData>::instance().data.checkDiagram(currentItem()->text(currentColumn()))) return;

    QString path = Singleton<GlobalData>::instance().project_path + '/' +
            project_ns::type_to_string(Singleton<GlobalData>::instance().data.getTypeDiagram(currentItem()->text(currentColumn()))) + '/' + currentItem()->text(currentColumn()) + ".png";

    emit diagram(path);
}
//вот тут самое интересное, откуда должны вызываться функции работы с библиотеками...
void TreeWidget::analyze()
{
    QLibrary* lib =load_library("analyzer");
    if (lib == nullptr)
        return;

    typedef QVector<Diagram> (*analyzer)(QVector<Diagram>);
    analyzer analyze = (analyzer) lib->resolve("analyze");
    if (analyze)
    {
        Singleton<GlobalData>::instance().data.m_diagrams = analyze(Singleton<GlobalData>::instance().data.m_diagrams);
        load();
        saveProject();
        emit update(Singleton<GlobalData>::instance().data.m_diagrams);
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
        map_actors = get_actors(Singleton<GlobalData>::instance().data.m_diagrams);
    }
    delete lib;
    return map_actors;
}
//перенесено в project_data
/*
QStringList TreeWidget::getRobustnessList()
{
    QStringList list;
    foreach (Diagram diag, Singleton<GlobalData>::instance().data.m_diagrams)
    {
        if (diag.m_type==Diagram::Type::robustness)
        {
            list.append(diag.m_name);
        }
    }
    return list;

}
*/
void TreeWidget::analyze_descirption()
{
    //тут такая же проблема с загрузкой...
   // load(description_analyze::analyze(Singleton<GlobalData>::instance().project_data,// додумать
        //                              Singleton<GlobalData>::instance().project_path + "/description.json"));
    saveProject();
    emit update(Singleton<GlobalData>::instance().data.m_diagrams);
}
//переехало в project_data
/*
void TreeWidget::checkDeleted(QVector<Diagram> diagrams)
{
    foreach (Diagram diag,Singleton<GlobalData>::instance().data.m_diagrams)
    {
        auto it = std::find_if(diagrams.begin(), diagrams.end(), [=](Diagram diagram) {
            return diagram.m_name == diag.m_name;
        });

        if (it == diagrams.end())
        {
            Singleton<GlobalData>::instance().data.m_deleted.insert(diag.m_name);
        }
        else
        {
            Singleton<GlobalData>::instance().data.m_deleted.remove(diag.m_name);
        }
    }
}*/

//с этой функцией я не знаю пока еще что делать....и закинуть ее по сути трудно да и оставить...
/*
void TreeWidget::updateTree(QVector<Diagram>& diagrams)
{
    for (int i=0;i<Singleton<GlobalData>::instance().data.m_diagrams.count();i++)
    {
        auto it = std::find_if(diagrams.begin(), diagrams.end(), [=](Diagram diagram) {
            return diagram.m_name == Singleton<GlobalData>::instance().data.m_diagrams[i].m_name;
        });

        if (it == diagrams.end()) {

            diagrams.push_back(Singleton<GlobalData>::instance().data.m_diagrams[i]);
        }
    }
}
*/
void TreeWidget::saveProject()
{
    project_ns::rewrite(Singleton<GlobalData>::instance().data.m_diagrams, Singleton<GlobalData>::instance().project_path + '/' + m_name);
    for (int i=0;i<Singleton<GlobalData>::instance().data.m_diagrams.count();i++)
    {
        project_ns::save(Singleton<GlobalData>::instance().data.m_diagrams[i]);
    }
}
