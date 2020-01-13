#include "umltexteditwidget.h"
#include <QDebug>

UMLTextEditWidget::UMLTextEditWidget(QWidget *parent, Diagram* diag)
    : QTextEdit(parent),
      m_diag(*diag)
{
    new UML_Highlighter(document());
    setFontPointSize(12);
    setFontFamily("Times New Roman");
    setAutoFormatting(QTextEdit::AutoAll);

    if (m_diag.m_text.isEmpty())
        setText("@startuml\n@enduml");
    else
        setText(m_diag.m_text);

    connect(this, SIGNAL(textChanged()),this,SLOT(textChange()));
}

void UMLTextEditWidget::textChange()
{
    m_diag.m_text = toPlainText();
}

