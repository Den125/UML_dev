#include "centralwidget.h"
#include <QGridLayout>
#include <QDebug>
#include "project.h"
#include <QFileDialog>
#include <QInputDialog>
#include "globaldata.h"

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
