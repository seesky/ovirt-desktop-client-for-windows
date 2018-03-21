#include "sessionstatusupdatethread.h"
#include <QTime>
#include "utils.h"
#include <QJsonParseError>
#include <QJsonDocument>



SessionStatusUpdateThread::SessionStatusUpdateThread()
{

}

SessionStatusUpdateThread::~SessionStatusUpdateThread()
{


}

void SessionStatusUpdateThread::run()
{




    //设置定时器，每5分钟执行一次
    QTimer *timer;
    timer = new QTimer();
    timer->start(300000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    this->exec();


    /**
    while(true)
    {


        //组装http请求的header
        //input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api", "GET");
        //input->headers.insert("Accept", "application/xml");
        //input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());

        //创建具体执行http请求的对象，里边用了QNetworkAccessManager
        //worker = new HttpRequestWorker();
        //将http请求的返回信号绑定到statusUpdate槽
        //connect(worker, &HttpRequestWorker::on_execution_finished, this, &SessionStatusUpdateThread::statusUpdate);
        //worker->execute(input);

        //sleep(5);
    }
    */
}

void SessionStatusUpdateThread::timeUpdate()
{
    emit sessionStatusUpdate(false);
    /**
    input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api", "GET");
    input->headers.insert("Accept", "application/xml");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());

    worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &SessionStatusUpdateThread::statusUpdate);
    worker->execute(input);
    */
}

void SessionStatusUpdateThread::statusUpdate()
{
    QString responseStr(worker->response);
    //responseStr = responseStr->prepend(worker->response);

    //qDebug(worker->response.data());

    if(!responseStr.contains("Unauthorized",Qt::CaseSensitive))
    {
        emit sessionStatusUpdate(true);
        qDebug("session ok");
    }else{
        emit sessionStatusUpdate(false);
        qDebug("session false");
    }


    //if(NULL != input) {qDebug("delete input"); delete input;  input = NULL;}
    //if(NULL != worker) {qDebug("delete worker"); delete worker;  worker = NULL;}
}


