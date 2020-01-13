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
    m_process->start(strCommand);
}

void Running::command_complete(int exitCode)
{
    FileName.chop(4);
    qDebug()<<exitCode<<FileName;
    emit complete(FileName+".png");
}
