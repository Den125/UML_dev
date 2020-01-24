#include "description_analyze.h"
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>


bool description_analyze::equalNames(const Diagram& lhs, const Diagram& rhs) {
    return lhs.m_name == rhs.m_name;
}
void description_analyze::insert(const Diagram& diag, QVector<Diagram>& diagrams) {
    auto it = std::find_if(diagrams.begin(), diagrams.end(), [&diag](Diagram& elem) {
        return equalNames(diag, elem);
    });

    if (it == diagrams.end()) {
        diagrams.push_back(diag);
    }

}
void description_analyze::update(const Diagram& diag, QVector<Diagram>& diagrams)
{
    auto it = std::find_if(diagrams.begin(), diagrams.end(), [&diag](Diagram& elem) {
        return equalNames(diag, elem);
    });
    if (it != diagrams.end()) {
        it->m_text = diag.m_text;
    }
}
QVector<Diagram> description_analyze::analyze(QVector<Diagram> &diagrams, QString path)
{
    QVector<Diagram> result(diagrams);
    QFile desc(path);
    if (!desc.exists())
    {
        return result;
    }
    if (!desc.open(QIODevice::ReadOnly))
    {
        return result;
    }
    QJsonDocument doc = QJsonDocument::fromJson(desc.readAll());
    if (doc.isNull())
    {
        // qDebug()<<"Ошибка чтения JSON";
        return result;
    }
    QStringList sequences;

    foreach (Diagram diag,diagrams)
    {
        if (diag.m_type==Diagram::Type::robustness)
        {
            QJsonObject root = doc.object().value(diag.m_name).toObject();
            int index_seq=0;
            Diagram new_diag;
            for (;index_seq<root.count()-3;index_seq++)
            {
                QString str,name;
                QJsonValue seq_list = root.value("list_seq" + QString::number(index_seq));
                if (seq_list.toObject().value("type_seq").toString() == "main")
                {
                    str.append("Главная последовательность:\n");
                    new_diag.m_name = diag.m_name + "_main";
                }
                else
                {
                    str.append("Альтернативная последовательность " + QString::number(index_seq) + ":\n");
                    new_diag.m_name = diag.m_name + "_alt" + QString::number(index_seq);
                }
                new_diag.m_type = Diagram::Type::sequence;
                insert(new_diag,result);
                int index_elem = 0;
                QJsonValue list_seq = seq_list.toObject().value("seq");
                if (!(list_seq.toObject().keys().empty()))
                {
                    for (;index_elem<list_seq.toObject().keys().count(); index_elem++)
                    {
                        QJsonValue elem_seq=list_seq.toObject().value(QString::number(index_elem));
                        str.append(QString::number(index_elem) + ") " + elem_seq.toString() + "\n");
                    }
                }
                sequences<<str;

                auto it = std::find_if(result.begin(), result.end(), [&new_diag](Diagram& elem) {
                        return equalNames(new_diag, elem);
                    });
                if (!it->m_text.isNull())
                {
                    QRegExp note("note left.*end note");
                    int index=0;
                    if ((index = note.indexIn(it->m_text,index))!=-1)
                    {
                        it->m_text.replace(index,note.matchedLength(),"note left\n "+str+"end note");
                    }
                    else
                    {
                        it->m_text.chop(7);
                        it->m_text.append("@startuml\nnote left\n " + str + "\nend note\n@enduml");
                    }
                }
                else
                {
                    it->m_text="@startuml\nnote left\n" + str + "end note\n@enduml";
                }
                update(Diagram(it->m_type,it->m_name,it->m_text), result);
            }
        }
    }
    return result;
}

