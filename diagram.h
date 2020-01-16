#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QString>

class Diagram {
public:
    enum class Type
    {
        use_case,
        robustness,
        sequence,
        classes
    };

    Diagram(Type type, const QString& name, const QString& text);
    Diagram() {}

    bool operator < (const Diagram& other) {
        if (m_type != other.m_type)
            return m_type < other.m_type;
        return m_name < other.m_name;
    }

    Type m_type;
    QString m_name;
    QString m_text;
};

#endif // DIAGRAM_H
