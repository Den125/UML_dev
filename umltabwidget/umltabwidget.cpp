#include "umltabwidget.h"
#include "umltexteditwidget.h"
#include <QTabBar>
#include <QMessageBox>
#include <QDebug>

UMLTabWidget::UMLTabWidget(QWidget* parent):
    QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

void UMLTabWidget::createTab(Diagram diag)
{
    if (isSuchTabOpened(diag.m_name))
        return;

    UMLTextEditWidget* tab = new UMLTextEditWidget(this, &diag);// diag.m_text);
    addTab(tab, diag.m_name);
    setCurrentIndex(count()-1);

    m_tabs.push_back(tab);
    connect(tab, SIGNAL(textChanged()),
            this, SLOT(changeText()));
}
bool UMLTabWidget::isSuchTabOpened(const QString &name)
{
    for (int i = 0; i < count(); i++)
    {
        if (tabText(i) == name)
            return true;
    }
    return false;
}

void UMLTabWidget::closeTab(int index)
{
    if (index >= m_tabs.length())
        return;

    if (tabText(index).contains('*') == true)
    {
        QMessageBox unsaved("Сохранить изменения", "В диаграмме " + tabText(index) + " имеются несохраненные изменения. Желаете сохранить?",
                            QMessageBox::Question, QMessageBox::Save, QMessageBox::No,
                            QMessageBox::Cancel, this);
        switch(unsaved.exec())
        {
            case QMessageBox::Save:
            {
                saveTab(index);
                break;
            }
            case QMessageBox::Cancel:
            {
                return;
            }
            default:
            {
                break;
            }
        }
    }

    m_tabs.remove(index);
    removeTab(index);
}

void UMLTabWidget::saveTab()
{
    if (count() == 0)
        return;

    emit save(Diagram(Diagram::Type::use_case, tabText(currentIndex()), m_tabs[currentIndex()]->toPlainText()));
    setTabText(currentIndex(), m_tabs[currentIndex()]->m_diag.m_name);
}

void UMLTabWidget::saveTab(int index)
{
    emit save(Diagram(Diagram::Type::use_case,tabText(index), m_tabs[index]->toPlainText()));
    setTabText(index, m_tabs[index]->m_diag.m_name);
}

void UMLTabWidget::changeText()
{
   int i=m_tabs.indexOf(dynamic_cast<UMLTextEditWidget*>(sender()));
   setTabText(i,m_tabs[i]->m_diag.m_name+'*');
}
