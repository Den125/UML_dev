#ifndef UMLTABWIDGET_H
#define UMLTABWIDGET_H
#include <QTabWidget>
#include "diagram.h"

class UMLTextEditWidget;

class UMLTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit UMLTabWidget(QWidget* parent = nullptr);
    Diagram currentDiagram();
public slots:
    void createTab(Diagram diag);
    void saveTab();
    void saveTab(int index);
    void saveTabs();
    void updateTabs(QVector<Diagram> diagrams);
signals:
    void save(Diagram diag);
private slots:
    void closeTab(int index);
    void changeText();
private:
    int getIndex(const QString& name);
    bool isSuchTabOpened(const QString& name);
    QVector<UMLTextEditWidget*> m_tabs;
};

#endif // UMLTABWIDGET_H
