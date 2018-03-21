#include "statusupdatethread.h"
#include <QTimer>
#include <QDateTime>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>



StatusUpdateThread::StatusUpdateThread()
{


}

StatusUpdateThread::~StatusUpdateThread()
{

}


void StatusUpdateThread::run()
{
    QTimer *timer;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5000);
    this->exec();

    //statusLabel->setText("Hello World!");
}


void StatusUpdateThread::update()
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("  yyyy年MM月dd日  hh:mm:ss  "); // 设置显示的格式

    QString address = QNetworkInterface::allAddresses()[1].toString();

    QNetworkConfigurationManager mgr;
    if(mgr.isOnline())
    {
        //QString localHostName = QHostInfo::localHostName();
        //QHostInfo info = QHostInfo::fromName(localHostName);
        Timestr += ("网络状态：良好  IP地址：");
        //Timestr += info.addresses()[0].toString();
        Timestr += address+" ";
    }else{
        Timestr += "网络状态: 断开";
    }
    emit updateStatus(Timestr);
}

