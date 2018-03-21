#include "pingthread.h"
#include <QString>
#include <QProcess>

QString ip;

PingThread::PingThread(QString targetIPAddress)
{
    ip = targetIPAddress;
}

PingThread::~PingThread()
{


}

void PingThread::run()
{

    QProcess p(0);
    p.start("ping " + ip);
    p.waitForStarted();
    p.waitForFinished();

    QString out = QString::fromLocal8Bit(p.readAllStandardOutput());

    emit pingResult(out);
}


