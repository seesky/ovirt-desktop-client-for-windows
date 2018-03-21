#include "vmstatusupdatethread.h"
#include <QTimer>
#include "httprequest.h"
#include "utils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

VmStatusUpdateThread::VmStatusUpdateThread(QString id)
{
    setVmId(id);
}

VmStatusUpdateThread::~VmStatusUpdateThread()
{

}

void VmStatusUpdateThread::run()
{

    QTimer *timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeSlot()));
    timer->start(5000);
    this->exec();

}

void VmStatusUpdateThread::timeSlot()
{
    input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/" + vmID, "GET");

    //input->headers.insert("Accept", "application/xml");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    input->headers.insert("Accept", "application/json");

    worker = new HttpRequestWorker(this);
    //connect(worker, &HttpRequestWorker::on_execution_finished, this, &VmStatusUpdateThread::update, Qt::QueuedConnection);
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &VmStatusUpdateThread::update);
    worker->execute(input);
}

void VmStatusUpdateThread::update(HttpRequestWorker *worker)
{
    //获取返回的所有VM的JSON描述信息
    //char *response = worker->response.data();

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(worker->response, &json_error);

    //qDebug(response);

    if(json_error.error == QJsonParseError::NoError)
    {

        if(parse_doucment.isObject())
        {
            //从JSON恢复中获取到总的VM列表对象
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("error"))
            {
                QJsonValue error = obj.take("error");
                if(error.isString())
                {
                    QString _error = error.toString();
                    QJsonValue errorCode = obj.take("error_code");
                    QString _errorCode = errorCode.toString();
                    //restAPIResultLabel->setText("  错误代码：" + _errorCode + "  错误内容：" + _error);
                    //qDebug("  错误代码：" + _errorCode + "  错误内容：" + _error);
                }
            }else{

                QHash<QString, QString> vmsStatusHash;
                vmsStatusHash.insert("status", obj.take("status").toString());
                vmsStatusHash.insert("systemType", obj.take("os").toObject().take("type").toString());
                emit updateStatus(vmsStatusHash);


            }
        }
    }
}

void VmStatusUpdateThread::setVmId(QString id)
{
    vmID = id;
}
