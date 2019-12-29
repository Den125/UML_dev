#ifndef TEXTEDITUMLWIDGET_H
#define TEXTEDITUMLWIDGET_H
#include <QTextEdit>
#include "umltabwidget/uml_highlighter.hpp"
#include "diagram.h"

class UMLTextEditWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit UMLTextEditWidget(QWidget *parent = nullptr, Diagram *diag=nullptr);
    Diagram m_diag;
private slots:
    void textChange();
};

#endif // TEXTEDITUMLWIDGET_H
