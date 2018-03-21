#include "execcmd.h"
#include <QProcess>

QString execmd;
long sleepTime;
ExecCMD::ExecCMD(QString cmd,long s)
{
    execmd = cmd;
    sleepTime = s;
}

ExecCMD::~ExecCMD()
{

}


void ExecCMD::run()
{
    sleep(sleepTime);
    QProcess p(0);
    p.start(execmd);
    p.waitForStarted();
    p.waitForFinished();

    QString out = execmd + "任务完成。" + QString::fromLocal8Bit(p.readAllStandardOutput());
    emit exec(out);
}
