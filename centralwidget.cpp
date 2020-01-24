#include "centralwidget.h"
#include <QDockWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include "project.h"
#include "globaldata.h"
#include "treewidget.h"
#include "umltabwidget/umltabwidget.h"
#include "picturewidget.h"
#include "toolbar.h"
#include "running.h"
#include "description/descriptionwidget.h"
#include <QDebug>

CentralWidget::CentralWidget(QWidget *parent)
    : QMainWindow(parent),      
      m_file(new QDockWidget("Активные файлы", this)),
      m_image(new QDockWidget("Диаграмма", this)),
      m_structure(new QDockWidget("Структура проекта", this))
{
    setWindowTitle("Средство проектирования ПО");

    addDockWidget(Qt::LeftDockWidgetArea, m_structure);
    addDockWidget(Qt::RightDockWidgetArea, m_file);
    addDockWidget(Qt::BottomDockWidgetArea, m_image);

    open_ini();

    m_tool = new ToolBar(this,m_image->isVisible(),m_file->isVisible(),m_structure->isVisible());
    addToolBar(m_tool);
    m_tool->activateActions(false);

    connect(m_tool, SIGNAL(newProject()), this, SLOT(newProject()));
    connect(m_tool, SIGNAL(openProject()), this, SLOT(openProject()));
    connect(m_tool, SIGNAL(closeProject()), this, SLOT(closeProject()));
    connect(m_tool, SIGNAL(buildDiagram()), this, SLOT(building()));
    connect(m_tool, SIGNAL(description()), this, SLOT(description()));
    connect(m_tool, SIGNAL(visibleStructure(bool)), this, SLOT(setStructureVisible(bool)));
    connect(m_tool, SIGNAL(visiblePicture(bool)), this, SLOT(setPictureVisible(bool)));
    connect(m_tool, SIGNAL(visibleFile(bool)), this, SLOT(setFileVisible(bool)));

}

void CentralWidget::newProject()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, "Выбор папки", "");
    if (dir.isNull())
        return;
    bool ok;
    QString name = QInputDialog::getText(nullptr, "Название проекта",
                                         "Введите название вашего проекта", QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !(name.isNull()))
    {
        if (!project_ns::create(dir,name))
            return;
        createWorkspace();
        Singleton<GlobalData>::instance().project_path=dir+"/"+name;
        m_tree->m_name = name;
        m_tree->load(project_ns::read(dir+"/"+name+"/"+name+".upp"));
        m_tool->activateActions(true);
        name.chop(4);
        setWindowTitle("Средство проектирования ПО - " + name);
    }

}

void CentralWidget::openProject()
{
    QString path = QFileDialog::getOpenFileName(nullptr, "Выбор файла проекта", "", "UMLParserProject (*.upp)");

    if (path.isNull())
        return;

    createWorkspace();

    m_tree->load(project_ns::read(path));

    Singleton<GlobalData>::instance().project_path = path.left(path.lastIndexOf('/'));

    m_tree->m_name = path.remove(0, path.lastIndexOf('/') + 1);
    setWindowTitle("Средство проектирования ПО - " + m_tree->m_name);
    m_tool->activateActions(true);
}

void CentralWidget::createWorkspace()
{
    m_tree = new TreeWidget(this);
    m_tabs = new UMLTabWidget(this);
    m_picture = new PictureWidget(this);
    m_run = new Running(this);

    m_structure->setWidget(m_tree);
    m_file->setWidget(m_tabs);
    m_image->setWidget(m_picture);

    connect(m_tool, SIGNAL(analyze()), this, SLOT(analyzing()));
    connect(m_tool, SIGNAL(saveDiagram()), m_tabs, SLOT(saveTab()));
    connect(m_tree, SIGNAL(selected(Diagram)), m_tabs, SLOT(createTab(Diagram)));
    connect(m_tree, SIGNAL(diagram(QString)), m_picture, SLOT(loadImage(QString)));
    connect(m_tabs, SIGNAL(save(Diagram)), m_tree, SLOT(saveDiagram(Diagram)));
    connect(m_tree, SIGNAL(update(QVector<Diagram>)), m_tabs, SLOT(updateTabs(QVector<Diagram>)));

    connect(m_run, SIGNAL(complete(QString)), m_picture, SLOT(loadImage(QString)));
    connect(m_run, SIGNAL(complete(QString)), this, SLOT(endBuilding()));
}

void CentralWidget::closeProject()
{
    m_tabs->saveTabs();
    m_tool->activateActions(false);
    setWindowTitle("Средство проектирования ПО");
    delete m_tree;
    delete m_tabs;
    delete m_picture;
    delete m_run;
    Singleton<GlobalData>::instance().project_path.clear();
}

void CentralWidget::building()
{
    m_tabs->saveTab();
    m_image->setEnabled(false);
    m_file->setEnabled(false);
    m_structure->setEnabled(false);
    m_tool->setEnabled(false);
    m_run->run(Singleton<GlobalData>::instance().project_path + '/'
               + project_ns::type_to_string(m_tabs->currentDiagram().m_type) + '/'
               + m_tabs->currentDiagram().m_name + ".txt");
}

void CentralWidget::endBuilding()
{
    m_image->setEnabled(true);
    m_file->setEnabled(true);
    m_structure->setEnabled(true);
    m_tool->setEnabled(true);
}

void CentralWidget::open_ini()
{
    QSettings set("config_uml.ini", QSettings::IniFormat);
    QMap<QString, QDockWidget*> map_dockwidgets={
        {"StructureWidget", m_structure},
        {"FileWidget", m_file},
        {"PictureWidget", m_image}
    };
    QMap<int,Qt::DockWidgetArea> map_area ={
        {1, Qt::LeftDockWidgetArea},
        {2, Qt::RightDockWidgetArea},
        {4, Qt::TopDockWidgetArea},
        {8, Qt::BottomDockWidgetArea}
    };
    foreach (QString name_widget, map_dockwidgets.keys())
    {
        set.beginGroup(name_widget);
        addDockWidget(map_area.value(set.value("DockPosition", 1).toInt()),
                      map_dockwidgets.value(name_widget));
        map_dockwidgets.value(name_widget)->setFloating(set.value("Floating", false).toBool());
        map_dockwidgets.value(name_widget)->setHidden(set.value("Hide", false).toBool());
        set.endGroup();
    }
    set.beginGroup("Window");
    if (set.value("Maximized", false).toBool() == true) showMaximized();
    else resize(set.value("Width", 800).toInt(), set.value("Height", 600).toInt());
    set.endGroup();
}

void CentralWidget::closeEvent(QCloseEvent* event)
{
    QSettings set("config_uml.ini", QSettings::IniFormat);
    QMap<QString,QDockWidget*> map_widgets = {
        {"StructureWidget", m_structure},
        {"FileWidget", m_file},
        {"PictureWidget", m_image}
    };
    foreach(QString name_widget, map_widgets.keys())
    {
        set.beginGroup(name_widget);
        set.setValue("DockPosition", dockWidgetArea(map_widgets.value(name_widget)));
        set.setValue("Floating", map_widgets.value(name_widget)->isFloating());
        set.setValue("Hide", map_widgets.value(name_widget)->isHidden());
        set.endGroup();
    }
    set.beginGroup("Window");
    set.setValue("Maximized", isMaximized());
    set.setValue("Width", width());
    set.setValue("Height", height());
    set.endGroup();
    if (!Singleton<GlobalData>::instance().project_path.isEmpty())
    {
        m_tabs->saveTabs();
    }
    event->accept();
}

void CentralWidget::setStructureVisible(bool visible)
{
    m_structure->setVisible(visible);
}

void CentralWidget::setFileVisible(bool visible)
{
    m_file->setVisible(visible);
}

void CentralWidget::setPictureVisible(bool visible)
{
    m_image->setVisible(visible);
}

void CentralWidget::analyzing()
{
    m_tabs->saveTabs();
    m_tree->analyze();
}

void CentralWidget::description()
{
    QStringList robustness_list=m_tree->getRobustnessList(), actors_list;
    if (robustness_list.isEmpty())
    {
        //вывод сообщения
        return;
    }
    QMap<QString,QStringList> map_actors=m_tree->getActors();
    actors_list=map_actors.keys();
    DescriptionWidget *m_descriptionWidget=new DescriptionWidget(this,robustness_list,map_actors);
    connect(m_descriptionWidget,SIGNAL(finished(int)),this,SLOT(closeDescription()));
    m_descriptionWidget->show();
}
void CentralWidget::closeDescription()
{
    qDebug()<<"check";
    m_tree->analyze_descirption();
}
