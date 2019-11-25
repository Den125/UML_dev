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

    Type m_type;
    QString m_name;
    QString m_text;
};

#endif // DIAGRAM_H
