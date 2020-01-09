#include "precedentwidget.h"
#include <QComboBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QFormLayout>
#include <QLabel>
#include <QTabWidget>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
PrecedentWidget::PrecedentWidget(QWidget *parent, QString text, QStringList list_actors)
    :QWidget(parent)
{
    m_name=text;
    m_data=Structures::loadPrecedent(m_name);
    QVBoxLayout *layout=new QVBoxLayout(this);
    QFormLayout *form_layout=new QFormLayout;
    QComboBox *face=new QComboBox(this);
    face->addItems(list_actors);
    QLineEdit *goal=new QLineEdit(this);
    goal->setText(m_data.goal);
    QLineEdit *usl=new QLineEdit(this);
    usl->setText(m_data.predUsl);
    connect(goal,SIGNAL(textChanged(QString)),this,SLOT(setGoal(QString)));
    connect(usl,SIGNAL(textChanged(QString)),this,SLOT(setUsl(QString)));
    connect(face,SIGNAL(currentTextChanged(QString)),this,SLOT(setFace(QString)));
    setFace(face->currentText());
    form_layout->addWidget(new QLabel("Действующее лицо:",this));
    form_layout->addWidget(face);
    form_layout->addWidget(new QLabel("Предусловие",this));
    form_layout->addWidget(usl);
    form_layout->addWidget(new QLabel("Цель",this));
    form_layout->addWidget(goal);
    QTabWidget *tab = new QTabWidget(this);
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setShowGrid(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab->addTab(table,"Главная");
    layout->addLayout(form_layout);
    layout->addWidget(tab);
    QPushButton *button_add_punkt = new QPushButton("+",this);
    connect(button_add_punkt,SIGNAL(clicked()),this,SLOT(add_punkt()));
    QPushButton *button_delete_punkt = new QPushButton("-",this);
    connect(button_delete_punkt,SIGNAL(clicked()),this,SLOT(delete_punkt()));
    QHBoxLayout *row_button=new QHBoxLayout;
    row_button->addWidget(button_add_punkt);
    row_button->addWidget(button_delete_punkt);
    layout->addLayout(row_button);
    QPushButton *button_add = new QPushButton("Добавить последовательность",this);
    connect(button_add,SIGNAL(clicked()),this,SLOT(add_seq()));
    QPushButton *button_delete = new QPushButton("Удалить последовательность",this);
    connect(button_delete,SIGNAL(clicked()),this,SLOT(delete_seq()));
    layout->addWidget(button_add);
    layout->addWidget(button_delete);
    setLayout(layout);
    set_into_table();
}

void PrecedentWidget::add_seq()
{
    QTabWidget *tab=findChild<QTabWidget*>();
    QWidget *widget=new QWidget(this);
    QVBoxLayout *layout=new QVBoxLayout();
    QHBoxLayout *Row=new QHBoxLayout();
    QLabel *label=new QLabel("Условие",this);
    QLineEdit *usl=new QLineEdit(this);
    Row->addWidget(label);
    Row->addWidget(usl);
    layout->addLayout(Row);
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setShowGrid(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(table);
    widget->setLayout(layout);
    tab->addTab(widget,"Альтернативная №"+QString::number(tab->count()));
}

void PrecedentWidget::delete_seq()
{
    QTabWidget *tab=findChild<QTabWidget*>();
    if (tab->currentIndex()==0)
        return;
    tab->removeTab(tab->currentIndex());
}

void PrecedentWidget::add_punkt()
{
    QTabWidget *tab=findChild<QTabWidget*>();
    QTableWidget *table;
    if (tab->currentIndex()==0)
    {
        table=dynamic_cast<QTableWidget*>(tab->currentWidget());
    }
    else
    {
        table=dynamic_cast<QTableWidget*>(tab->currentWidget()->findChild<QTableWidget*>());
    }
    table->insertRow(table->rowCount());
}

void PrecedentWidget::delete_punkt()
{
    QTabWidget *tab=findChild<QTabWidget*>();
    QTableWidget *table;
    if (tab->currentIndex()==0)
    {
        table=dynamic_cast<QTableWidget*>(tab->currentWidget());
    }
    else
    {
        table=dynamic_cast<QTableWidget*>(tab->currentWidget()->findChild<QTableWidget*>());
    }
    table->removeRow(table->currentRow());
}

void PrecedentWidget::setGoal(QString goal)
{
    m_data.goal=goal;
}

void PrecedentWidget::setUsl(QString usl)
{
    m_data.predUsl=usl;
}

void PrecedentWidget::setFace(QString face)
{
    m_data.activeFace=face;
}

void PrecedentWidget::savePrecedent()
{
    QTabWidget *tab=findChild<QTabWidget*>();
    m_data.seqVect.clear();
    for (int i=0;i<tab->count();i++)
    {
        Structures::sequenceData seq;
        QTableWidget *table;
        if (i==0)
        {
            seq.type=Structures::main_seq;
            table=dynamic_cast<QTableWidget*>(tab->widget(i));
        }
        else
        {
            seq.type=Structures::alt_seq;
            table=dynamic_cast<QTableWidget*>(tab->widget(i)->findChild<QTableWidget*>());
            QLineEdit *line=dynamic_cast<QLineEdit*>(tab->widget(i)->findChild<QLineEdit*>());
            seq.usl=line->text();
        }
        for (int j=0;j<table->rowCount();j++)
        {
            if (table->item(j,0)!=NULL)
                seq.elem_seq.push_back(table->item(j,0)->text());
        }
        m_data.seqVect.push_back(seq);
    }
    Structures::savePrecedent(m_name,m_data);
}

void PrecedentWidget::set_into_table()
{
   QTabWidget *tab=findChild<QTabWidget*>();
   for (int i=0;i<m_data.seqVect.count();i++)
   {
       QTableWidget *table;
       if (i==0)
       {
           table=dynamic_cast<QTableWidget*>(tab->widget(i));
       }
       else
       {
           add_seq();
           table=dynamic_cast<QTableWidget*>(tab->widget(i)->findChild<QTableWidget*>());
           QLineEdit *line=dynamic_cast<QLineEdit*>(tab->widget(i)->findChild<QLineEdit*>());
           line->setText(m_data.seqVect[i].usl);
       }

       for (int j=0;j<m_data.seqVect[i].elem_seq.count();j++)
       {
           table->insertRow(table->rowCount());
           table->setItem(j,0,new QTableWidgetItem(m_data.seqVect[i].elem_seq[j]));
       }
   }
}
