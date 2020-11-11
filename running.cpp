#include "running.h"
#include <QDebug>

Running::Running(QObject *parent)
    : QObject(parent) {
    m_process=new QProcess(this);
    connect(m_process,SIGNAL(finished(int)),this,SLOT(command_complete(int)));
}

void Running::run(QString fileName)
{
    FileName=fileName;
    QString strCommand;
    if(QSysInfo::productType()=="windows")
        strCommand = "cmd /C ";    
    strCommand += "java -jar plantuml.jar "+FileName;
    qDebug()<<strCommand;
    m_process->start(strCommand);
}

void Running::command_complete(int exitCode)
{
    if (exitCode!=0)
    {
        qDebug()<<FileName<<exitCode<<m_process->errorString();
    }
    FileName.chop(4);
    emit complete(FileName+".png");
}
