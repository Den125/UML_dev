#include "structures.h"
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

namespace structures
{
QString type_seq(SequenceType type_p)
{
    QMap<SequenceType,QString> map_type ={
        {main_seq,"main"},{alt_seq,"alt"}
    };
    return map_type.value(type_p);
}

SequenceType type_seq(QString type_p)
{
    QMap<QString,SequenceType> map_type ={
        {"main",main_seq},{"alt",alt_seq}
    };
    return map_type.value(type_p);
}

void savePrecedent(QString name, precedentData data)
{
     QJsonObject key_elem;
     key_elem.insert("active_face",data.activeFace);
     key_elem.insert("goal",data.goal);
     key_elem.insert("uslovie",data.predUsl);
     for (int i=0;i<data.seqVect.count();i++)
     {
         QJsonObject seq;
         seq.insert("type_seq",type_seq(data.seqVect[i].type));
         seq.insert("usl_seq",data.seqVect[i].usl);
         QJsonObject seq_vect;
         for (int j=0;j<data.seqVect[i].elem_seq.count();j++)
         {
             seq_vect.insert(QString::number(j),data.seqVect[i].elem_seq[j]);
         }
         seq.insert("seq",seq_vect);
         key_elem.insert("list_seq"+QString::number(i),seq);
     }
     QFile *JSON= new QFile(Singleton<GlobalData>::instance().project_path+"/description.json");
     QJsonDocument doc;
     if (JSON->open(QIODevice::ReadOnly))
     {
         doc=QJsonDocument::fromJson(JSON->readAll());
         JSON->close();
     }
     QJsonObject root = doc.object();
     root.insert(name,key_elem);
     if (JSON->open(QIODevice::WriteOnly))
     {
         JSON->write(QJsonDocument(root).toJson());
         JSON->close();
     }
}

precedentData loadPrecedent(QString name)
{
    precedentData new_precedent;
    QFile *JSON= new QFile(Singleton<GlobalData>::instance().project_path+"/description.json");
    QJsonDocument doc;
    if (!(JSON->open(QIODevice::ReadOnly)))
    {
        return new_precedent;
    }
    doc=QJsonDocument::fromJson(JSON->readAll());
    JSON->close();
    QJsonObject root = doc.object();
    QJsonValue jsvalue = root.value(name);
    QStringList keys=jsvalue.toObject().keys();
    if (!keys.empty())
    {
        new_precedent.predUsl=jsvalue.toObject().value("uslovie").toString();
        new_precedent.goal=jsvalue.toObject().value("goal").toString();
        new_precedent.activeFace=jsvalue.toObject().value("active_face").toString();
        int index_seq=0;
        for (;index_seq<keys.count()-3;index_seq++)
        {
            QJsonValue seq_list=jsvalue.toObject().value("list_seq"+QString::number(index_seq));
            sequenceData seq;
            seq.type=type_seq(seq_list.toObject().value("type_seq").toString());
            seq.usl=seq_list.toObject().value("usl_seq").toString();
            int index_elem=0;
            QJsonValue list_seq=seq_list.toObject().value("seq");
            if (!(list_seq.toObject().keys().empty()))
            {
                for (;index_elem<list_seq.toObject().keys().count();index_elem++)//проблема здесь
                {
                    QJsonValue elem_seq=list_seq.toObject().value(QString::number(index_elem));
                    seq.elem_seq.push_back(elem_seq.toString());
                }
            }
            new_precedent.seqVect.push_back(seq);
        }
    }
    return new_precedent;
}
}
