#include "uml_highlighter.hpp"
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QDebug>

UML_Highlighter::UML_Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    tagsFormat.setForeground(Qt::blue);
    QStringList keywordPatterns;
    keywordPatterns <<"^[c,C]lass\\b"<<"^[a,A]ctor\\b"<<"^[b,B]oundary\\b"<<"^[c,C]ontrol\\b"<<"[u,U]secase\\b"<<
                      "^[e,E]ntity\\b"<<"^[d,D]atabase\\b"<<"^[c,C]ollections\\b"<<"^[p,P]articipant\\b"<<"\\b[n,N]ote\\b";
    for (const QString &pattern : keywordPatterns)
    {

        rule.pattern = QRegExp(pattern);
        rule.format = tagsFormat;
        TagRules.append(rule);
    }
    QStringList symbols;
    symbols <<"-{1,3}>"<<"[*,o,#,x,},+,/^]-{2}"<<"(<|)?-{2}"<<"<-{1,3}"<<"-{2}[*,o,#,x,},+,/^]"
           <<"[.]{2}([|])?>"<<"<([|])?[.]{2}"<<"[.]{2}";
    SymbFormat.setForeground(Qt::red);
    for (const QString &pattern : symbols)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = SymbFormat;
        SymbRules.append(rule);
    }
    label = QRegExp(" : ");
    labelFormat.setForeground(Qt::green);
}

void UML_Highlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(UML_Highlighter::None);
    if (text.length()==0) return;
    int index = 0;
   // qDebug()<<text;
    for (const HighlightingRule &rule : SymbRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    index=0;
    for (const HighlightingRule &rule : TagRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
          //  qDebug()<<index<<"word: "<<text;
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    int l_index=label.indexIn(text,0);
    int length = 0;
    if (l_index!=-1){
        index=l_index;
        while (index!=text.length())
        {
            index++;
            length++;
        }
        setFormat(l_index + 1,length,labelFormat);
    }
}
