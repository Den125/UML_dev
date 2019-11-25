#include "project.h"
#include <QFile>
#include <QMap>
#include <QTextStream>

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
    file.open(QIODevice::ReadOnly); // TODO check
    QTextStream textStream(&file);

    return Diagram(type, name, textStream.readAll());
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

}
