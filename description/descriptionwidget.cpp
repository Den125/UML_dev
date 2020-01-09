#include "descriptionwidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCloseEvent>
#include <QDebug>
#include "precedentwidget.h"

DescriptionWidget::DescriptionWidget(QWidget *parent, QStringList list_precedents, QMap<QString, QStringList> list_actors)
    :QDialog(parent)
{
    m_list_actors=list_actors;
    setWindowTitle("Описание");
    QVBoxLayout *layout= new QVBoxLayout(this);
    QHBoxLayout *Row=new QHBoxLayout;
    QComboBox *combo(new QComboBox(this));
    combo->addItems(list_precedents);
    connect(combo,SIGNAL(currentTextChanged(QString)),this,SLOT(changePrecedent(QString)));
    m_widget_seq=new PrecedentWidget(this,combo->currentText(),list_actors.value(combo->currentText()));
    QLabel *label=new QLabel("Прецедент:",this);
    Row->addWidget(label);
    Row->addWidget(combo);
    layout->addLayout(Row);
    layout->addWidget(m_widget_seq);
    setLayout(layout);
}

void DescriptionWidget::changePrecedent(QString text)
{
    m_widget_seq->savePrecedent();
    delete m_widget_seq;
    m_widget_seq=new PrecedentWidget(this,text,m_list_actors.value(text));
    layout()->addWidget(m_widget_seq);
}

void DescriptionWidget::closeEvent(QCloseEvent *event)
{
    m_widget_seq->savePrecedent();
    emit finished(0);
    event->accept();
}
