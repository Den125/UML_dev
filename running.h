#ifndef RUNNING_H
#define RUNNING_H

#include <QString>
#include <QProcess>

class Running:public QObject

{
    Q_OBJECT
public:
    void run(QString fileName);
    explicit Running(QObject *parent = 0);
signals:
    void complete(QString name);
private slots:
    void command_complete(int exitCode);
private:
    QProcess *m_process;
    QString FileName;
};

#endif // RUNNING_H
