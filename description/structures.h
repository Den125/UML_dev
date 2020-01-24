#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <QVector>
#include "globaldata.h"

namespace structures
{
    enum SequenceType
    {
        main_seq,
        alt_seq
    };

    struct sequenceData
    {
        QString usl;
        QVector<QString> elem_seq;
        SequenceType type;
    };

    struct precedentData
    {
        QString activeFace,goal,predUsl;
        QVector<sequenceData> seqVect;
    };

    SequenceType type_seq(QString type_p);
    QString type_seq(SequenceType type_d);
    void savePrecedent(QString name, precedentData);
    precedentData loadPrecedent(QString name);
};


#endif // STRUCTURES_H
