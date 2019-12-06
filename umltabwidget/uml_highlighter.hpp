#ifndef UML_HIGHLIGHTER_HPP
#define UML_HIGHLIGHTER_HPP

#include <QSyntaxHighlighter>

class QTextDocument;
class QTextCharFormat;

class UML_Highlighter:public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit UML_Highlighter(QTextDocument *parent=nullptr);
protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
private:
    enum States {
        None,
        Case,
        Label
    };

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QRegExp label;
    QRegExp desc;
    QTextCharFormat labelFormat;
    QTextCharFormat DescFormat;
    QVector<HighlightingRule> TagRules;
    QVector<HighlightingRule> SymbRules;
    QTextCharFormat tagsFormat;
    QTextCharFormat SymbFormat;
};

#endif // UML_HIGHLIGHTER_HPP
