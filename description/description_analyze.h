#ifndef DESCRIPTION_ANALYZE_H
#define DESCRIPTION_ANALYZE_H

#include <QVector>
#include "diagram.h"

namespace description_analyze {
    QVector<Diagram> analyze(QVector<Diagram> &diagrams,QString path);
    bool equalNames(const Diagram& lhs, const Diagram& rhs);
    void insert(const Diagram& diag, QVector<Diagram>& diagrams);
    void update(const Diagram& diag, QVector<Diagram>& diagrams);
}

#endif // DESCRIPTION_ANALYZE_H
