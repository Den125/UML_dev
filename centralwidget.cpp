#include "centralwidget.h"
#include <QGridLayout>
#include <QCloseEvent>
#include <QDebug>
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

CentralWidget::CentralWidget(QWidget *parent)
    : QMainWindow(parent),
      m_tree(new TreeWidget(this)),
      m_tabs(new UMLTabWidget(this)),
      m_picture(new PictureWidget(this)),
      m_file(new QDockWidget("Активные файлы", this)),
      m_image(new QDockWidget("Диаграмма", this)),
      m_structure(new QDockWidget("Структура проекта", this)),
      m_tool(new ToolBar(this)),
      m_run(new Running(this))
{
    addToolBar(m_tool);
    m_tool->activateActions(false);
    m_structure->setWidget(m_tree);
    m_file->setWidget(m_tabs);
    m_image->setWidget(m_picture);

    addDockWidget(Qt::LeftDockWidgetArea, m_structure);
    addDockWidget(Qt::RightDockWidgetArea, m_file);
    addDockWidget(Qt::BottomDockWidgetArea, m_image);

    connect(m_tree, SIGNAL(selected(Diagram)), m_tabs, SLOT(createTab(Diagram)));
    connect(m_tree, SIGNAL(diagram(QString)), m_picture, SLOT(loadImage(QString)));
    connect(m_tabs, SIGNAL(save(Diagram)), m_tree, SLOT(saveDiagram(Diagram)));

    connect(m_tool, SIGNAL(newProject()), this, SLOT(newProject()));
    connect(m_tool, SIGNAL(openProject()), this, SLOT(openProject()));
    connect(m_tool, SIGNAL(closeProject()), this, SLOT(closeProject()));
    connect(m_tool, SIGNAL(saveDiagram()), m_tabs, SLOT(saveTab()));
    connect(m_tool, SIGNAL(buildDiagram()), this, SLOT(building()));
    connect(m_tool, SIGNAL(saveAll()), m_tabs, SLOT(saveTabs()));
    connect(m_run, SIGNAL(complete(QString)), m_picture, SLOT(loadImage(QString)));
    connect(m_tool, SIGNAL(visible(int)), this, SLOT(showOrHide(int)));
    open_ini();
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

        Singleton<GlobalData>::instance().project_path=dir+"/"+name;
        m_tree->m_name = name;
        m_tree->load(project_ns::read(dir+"/"+name+"/"+name+".upp"));
        m_tool->activateActions(true);
    }

}

void CentralWidget::openProject()
{
    QString path = QFileDialog::getOpenFileName(nullptr, "Выбор файла проекта", "", "UMLParserProject (*.upp)");
    if (path.isNull())
        return;

    m_tree->load(project_ns::read(path));

    Singleton<GlobalData>::instance().project_path = path.left(path.lastIndexOf('/'));

    m_tree->m_name = path.remove(0, path.lastIndexOf('/') + 1);

    m_tool->activateActions(true);
}

void CentralWidget::closeProject()//не закончен
{
   // m_tabs->checkSaved();
    m_tool->activateActions(false);
}

void CentralWidget::building()//не закончен
{
    m_tabs->saveTab();
    m_run->run(Singleton<GlobalData>::instance().project_path + '/'
               + project_ns::type_to_string(m_tabs->currentDiagram().m_type) + '/'
               + m_tabs->currentDiagram().m_name + ".txt");
}

void CentralWidget::open_ini()
{
    QSettings set("config_uml.ini",QSettings::IniFormat);
    QMap<QString,QDockWidget*> map_dockwidgets={
        {"StructureWidget",m_structure},
        {"FileWidget",m_file},
        {"PictureWidget",m_image}
    };
    QMap<int,Qt::DockWidgetArea> map_area ={
        {1,Qt::LeftDockWidgetArea},{2,Qt::RightDockWidgetArea},
        {4,Qt::TopDockWidgetArea},{8,Qt::BottomDockWidgetArea}
    };
    foreach (QString name_widget,map_dockwidgets.keys())
    {
        set.beginGroup(name_widget);
        addDockWidget(map_area.value(set.value("DockPosition",1).toInt()),
                      map_dockwidgets.value(name_widget));
        map_dockwidgets.value(name_widget)->setFloating(set.value("Floating", false).toBool());
        map_dockwidgets.value(name_widget)->setHidden(set.value("Hide", false).toBool());
        set.endGroup();
    }
    set.beginGroup("Window");
    if (set.value("Maximized",false).toBool()==true) showMaximized();
    else resize(set.value("Width",800).toInt(),set.value("Height",600).toInt());
    set.endGroup();
}

void CentralWidget::closeEvent(QCloseEvent* event)
{
    QSettings set("config_uml.ini",QSettings::IniFormat);
    QMap<QString,QDockWidget*> map_widgets = {
        {"StructureWidget",m_structure},
        {"FileWidget",m_file},
        {"PictureWidget",m_image}
    };
    foreach(QString name_widget,map_widgets.keys())
    {
        set.beginGroup(name_widget);
        set.setValue("DockPosition",dockWidgetArea(map_widgets.value(name_widget)));
        set.setValue("Floating",map_widgets.value(name_widget)->isFloating());
        set.setValue("Hide",map_widgets.value(name_widget)->isHidden());
        set.endGroup();
    }
    set.beginGroup("Window");
    set.setValue("Maximized",isMaximized());
    set.setValue("Width",width());
    set.setValue("Height",height());
    set.endGroup();
  /*  if (!m_project->getName().isEmpty())
    {
        m_tabWidget->saveAllTabs();
        saveProject();
        m_project->save_Data();
    }*/
    event->accept();
}

void CentralWidget::showOrHide(int ind)
{
    QMap<int,QDockWidget*> map_widgets = {
        {1,m_file},
        {2,m_structure},
        {3,m_image}
    };
    map_widgets.value(ind)->setVisible(!map_widgets.value(ind)->isVisible());
}
