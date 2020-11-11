#include "project_data.h"

Project_data::Project_data()
{

}
void Project_data::load(QVector<Diagram> diagrams)
{
    // QVector<Diagram> project=m_diagrams;
     //m_diagrams.clear();
     checkDeleted(diagrams);
     for (int i=0;i<diagrams.count();i++)
     {
         auto it = std::find_if(m_diagrams.begin(), m_diagrams.end(), [=](Diagram diagram) {
             return diagram.m_name == diagrams[i].m_name;
         });

         if (it == m_diagrams.end()) {

             m_diagrams.push_back(diagrams[i]);
         }
     }
   // m_diagrams = diagrams;
     sort();

}

void Project_data::sort() //сортировка диаграмм
{
    std::sort(m_diagrams.begin(), m_diagrams.end(), [](const Diagram& a, const Diagram& b) {
        if (a.m_type != b.m_type) {
            return int (a.m_type) < int (b.m_type);
        }

        return a.m_name < b.m_name;
    });
}

void Project_data::clear() //очистка данных
{
    m_diagrams.clear();
    m_deleted.clear();
}

Diagram Project_data::getDiagramByName(QString name) //получение диаграммы по имени...
{
    if (!checkDiagram(name)) return Diagram();

    for (int i=0;i<m_diagrams.count();i++)
    {
        if (m_diagrams[i].m_name == name)
            return m_diagrams[i];
    }
    return Diagram();
}

void Project_data::UpdateDiagram(Diagram diag)
{
    for (int i=0;i<m_diagrams.count();i++)
    {
        if (m_diagrams[i].m_name == diag.m_name)
        {
            m_diagrams[i].m_text = diag.m_text;
            break;
        }

    }
}

QImage Project_data::getImageByName(QString name) //это получение изображения по имени, но пока еще в разработке...ибо есть нюансы..
{

}

Diagram::Type Project_data::getTypeDiagram(QString name) //получение типа диаграммы
{
    for (int i = 0; i < m_diagrams.count(); i++)
    {
        if (m_diagrams[i].m_name == name)
        {
            return m_diagrams[i].m_type;
        }
    }
}

bool Project_data::checkDiagram(QString name)  //проверка существования диаграммы
{
    auto it = std::find_if(m_diagrams.begin(), m_diagrams.end(), [=](Diagram diagram) {
        return diagram.m_name == name;
    });

    if (it == m_diagrams.end()) {
        return false;
    }

    return true;
}

bool Project_data::deleteDiagram(QString name) //удаление диаграммы
{
    if (!checkDiagram(name)) return false;

    for (int i = 0; i < m_diagrams.count(); i++)
    {
        if (m_diagrams[i].m_name == name)
        {
            m_diagrams.remove(i);
            return true;
        }
    }

    return false;
}

void Project_data::checkDeleted(QVector<Diagram> diagrams) //проверка удаленных диаграмм
{
    foreach (Diagram diag,m_diagrams)
    {
        auto it = std::find_if(diagrams.begin(), diagrams.end(), [=](Diagram diagram) {
            return diagram.m_name == diag.m_name;
        });

        if (it == diagrams.end())
        {
            m_deleted.insert(diag.m_name);
        }
        else
        {
            m_deleted.remove(diag.m_name);
        }
    }
}

QStringList Project_data::getRobustnessList()
{
    QStringList list;
    foreach (Diagram diag, m_diagrams)
    {
        if (diag.m_type==Diagram::Type::robustness)
        {
            list.append(diag.m_name);
        }
    }
    return list;

}
