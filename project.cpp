#include "project.h"
#include <QFile>
#include <QMap>
#include <QTextStream>
#include "globaldata.h"
#include <QDir>

using namespace std;

namespace project_ns
{

Diagram read_diag(QString diag_path);
Diagram::Type string_to_type(QString type);

QVector<Diagram> read(QString file_name) {
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);

    QVector<Diagram> diagrams;
    while (!textStream.atEnd()) {
        QString diag_path;
        textStream >> diag_path;

        if (diag_path.isEmpty())
            break;

        Diagram diag(read_diag(diag_path));
        diagrams.push_back(diag);
    }
    return diagrams;
}

Diagram read_diag(QString diag_path) {
    QString name = diag_path.split("/")[1];
    Diagram::Type type = string_to_type(diag_path.split("/")[0]);
    QString text;

    QFile file(diag_path + ".txt");
    if (file.open(QIODevice::ReadOnly)) // TODO check
    {
        QTextStream textStream(&file);
        return Diagram(type, name, textStream.readAll());
    }

    return Diagram(type, name, "");
}

Diagram::Type string_to_type(QString type) {
    static const std::map<QString, Diagram::Type> types({
        make_pair("use_case", Diagram::Type::use_case),
        make_pair("robustness", Diagram::Type::robustness),
        make_pair("sequence",  Diagram::Type::sequence),
        make_pair("classes", Diagram::Type::classes)
    });
    auto it = types.find(type);
    if (it == types.end())
        throw std::runtime_error("can't convert string to Diagram::Type: " + type.toLatin1());

    return it->second;
}

QString type_to_string(Diagram::Type type) {
    static const std::map<Diagram::Type, QString> types({
        make_pair(Diagram::Type::use_case, "use_case"),
        make_pair(Diagram::Type::robustness, "robustness"),
        make_pair(Diagram::Type::sequence, "sequence"),
        make_pair(Diagram::Type::classes, "classes")
    });

    auto it = types.find(type);
    if (it == types.end())
        throw std::runtime_error("can't convert Diagram::Type to sting");

    return it->second;
}

void rewrite(QVector<Diagram> vector, QString file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream textStream(&file);
    foreach(Diagram item,vector)
    {
        textStream << type_to_string(item.m_type) << '/' <<
                      item.m_name << endl;
    }
}
void save(Diagram diag)
{
    QDir directory(Singleton<GlobalData>::instance().project_path);
    directory.mkdir(project_ns::type_to_string(diag.m_type));

    QFile diagram (directory.path() + '/' + type_to_string(diag.m_type)+ '/' + diag.m_name + ".txt");
    if (!diagram.open(QIODevice::WriteOnly))
        return;
    diagram.resize(0);

    QTextStream stream(&diagram);
    stream << diag.m_text;
}
}
