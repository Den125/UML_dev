#ifndef USERSDOCKWIDGET_H
#define USERSDOCKWIDGET_H
#include <QDockWidget>

class UsersDockWidget: public QDockWidget
{
    Q_OBJECT
public:
    explicit UsersDockWidget(QMainWindow *parent = nullptr, QString name="");
private slots:
    void dockWidget_customContextMenuRequested(QPoint pos);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
protected:
    QMainWindow* m_parent;
};

#endif // USERSDOCKWIDGET_H
