#include "vmele.h"
#include "ui_vmele.h"
#include "httprequest.h"
#include "utils.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QLatin1String>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "windows.h"

VMEle::VMEle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VMEle)
{
    ui->setupUi(this);
    //登陆等待动画
    pIndicator = new QProgressIndicator(this);
    pIndicator->setColor(Qt::white);

    firstUpdate = true;

    //刚获取到机器需要跟新状态，此时所有按钮灰色，不能点击
    ui->startPushButton->setDisabled(true);
    ui->startPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/start-dark.png);}");

    ui->connectPushButton->setDisabled(true);
    ui->connectsPushButton->setDisabled(true);
    ui->connectsPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/connect-dark.png);}");

    ui->restartPushButton->setDisabled(true);
    ui->restartPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/restart-dark.png);}");

    ui->shutdownPushButton->setDisabled(true);
    ui->shutdownPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/shutdown-dark.png);}");


    pIndicator->startAnimation();
}

VMEle::~VMEle()
{
    delete ui;
}

void VMEle::startStatusUpdate()
{



    vmStatusUpdateThread = new VmStatusUpdateThread(vmID);
    connect(vmStatusUpdateThread, &VmStatusUpdateThread::updateStatus, this, &VMEle::statusUpdate, Qt::DirectConnection);
    //connect(vmStatusUpdateThread, &VmStatusUpdateThread::updateStatus, this, &VMEle::statusUpdate);
    //vmStatusUpdateThread->setVmId(vmID);
    vmStatusUpdateThread->start();
    ui->vmNameLabel->setText(vmName + "  正在更新状态");
}

void VMEle::setVmId(QString id)
{
    this->vmID = id;
}

void VMEle::setVmName(QString name)
{
    vmName = name;
    ui->vmNameLabel->setText(name);
}

void VMEle::statusUpdate(QHash<QString, QString> vmStatusHash)
{
    if(firstUpdate == true){
        firstUpdate = false;
        pIndicator->stopAnimation();
    }

    if(vmStatusHash.value("systemType") == "windows_7"){
        ui->connectPushButton->setStyleSheet("border-image: url(:/image/image/win7-light.png);");
    }else{
        ui->connectPushButton->setStyleSheet("border-image: url(:/image/image/win8-light.png);");
    }

    if(vmStatusHash.value("status") == "down")
    {
        ui->vmNameLabel->setText(vmName + "  已关机");

        ui->startPushButton->setDisabled(false);
        ui->startPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/start-light.png);}");

        ui->connectPushButton->setDisabled(true);
        ui->connectsPushButton->setDisabled(true);
        ui->connectsPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/connect-dark.png);}");

        ui->restartPushButton->setDisabled(true);
        ui->restartPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/restart-dark.png);}");

        ui->shutdownPushButton->setDisabled(true);
        ui->shutdownPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/shutdown-dark.png);}");
    }else if(vmStatusHash.value("status") == "up"){
        ui->vmNameLabel->setText(vmName + "  已开机");
        ui->startPushButton->setDisabled(true);
        ui->startPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/start-dark.png);}");

        ui->connectPushButton->setDisabled(false);
        ui->connectsPushButton->setDisabled(false);
        ui->connectsPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/connect-light.png);}");

        ui->restartPushButton->setDisabled(false);
        ui->restartPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/restart-light.png);}");

        ui->shutdownPushButton->setDisabled(false);
        ui->shutdownPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/shutdown-light.png);}");
    }else if(vmStatusHash.value("status") == "powering_up"){
        ui->vmNameLabel->setText(vmName + "  正在开机");
        ui->startPushButton->setDisabled(true);
        ui->startPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/start-dark.png);}");

        ui->connectPushButton->setDisabled(true);
        ui->connectsPushButton->setDisabled(true);
        ui->connectsPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/connect-dark.png);}");

        ui->restartPushButton->setDisabled(true);
        ui->restartPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/restart-dark.png);}");

        ui->shutdownPushButton->setDisabled(true);
        ui->shutdownPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/shutdown-dark.png);}");
    }else if(vmStatusHash.value("status") == "powering_down"){
        ui->vmNameLabel->setText(vmName + "  正在关机");
        ui->startPushButton->setDisabled(true);
        ui->startPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/start-dark.png);}");

        ui->connectPushButton->setDisabled(true);
        ui->connectsPushButton->setDisabled(true);
        ui->connectsPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/connect-dark.png);}");

        ui->restartPushButton->setDisabled(true);
        ui->restartPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/restart-dark.png);}");

        ui->shutdownPushButton->setDisabled(true);
        ui->shutdownPushButton->setStyleSheet("QPushButton {border-image: url(:/image/image/shutdown-dark.png);}");
    }
}

void VMEle::on_startPushButton_clicked()
{
    pIndicator->startAnimation();
    HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/start", "POST");

    //input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms", "GET");
    //input->headers.insert("Accept", "application/xml");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    input->headers.insert("Accept", "application/xml");
    //input->headers.insert("Content-Type", "application/xml; charset=UTF-8");

    input->request_body = QString("<action/>").toLatin1();

    worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::startProcessSlot);
    worker->execute(input);
}

void VMEle::startProcessSlot(HttpRequestWorker *worker)
{
    char *response = worker->response.data();

    qDebug(qPrintable(worker->error_str));
    pIndicator->stopAnimation();

    /**
    QString str = QString(QLatin1String(response));
    QFile outFile("debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << str << endl;
    */
}

void VMEle::on_shutdownPushButton_clicked()
{
    pIndicator->startAnimation();
    HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/shutdown", "POST");

    //input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms", "GET");
    //input->headers.insert("Accept", "application/xml");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    input->headers.insert("Accept", "application/xml");
    //input->headers.insert("Content-Type", "application/xml; charset=UTF-8");

    input->request_body = QString("<action/>").toLatin1();

    worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::startProcessSlot);
    worker->execute(input);
}

void VMEle::on_restartPushButton_clicked()
{
    HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/restart", "POST");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    input->headers.insert("Accept", "application/xml");

    input->request_body = QString("<action/>").toLatin1();

    worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::startProcessSlot);
    worker->execute(input);
}

void VMEle::on_connectsPushButton_clicked()
{
    if(Utils::getConnectMode() == "spice"){
        pIndicator->startAnimation();
        HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/graphicsconsoles", "GET");
        input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
        input->headers.insert("Accept", "application/json");

        input->request_body = QString("<action/>").toLatin1();

        worker = new HttpRequestWorker();
        connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::spiceConnectSlot);
        worker->execute(input);
    }else{
        pIndicator->startAnimation();
        HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/nics", "GET");
        input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
        input->headers.insert("Accept", "application/json");

        input->request_body = QString("<action/>").toLatin1();

        worker = new HttpRequestWorker();
        connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::rdpConnectSlot);
        worker->execute(input);
    }

}

void VMEle::rdpConnectSlot(HttpRequestWorker *worker)
{
    QJsonArray nicList;
    QString ipAddress;
    //获取返回的所有VM的JSON描述信息
    char *response = worker->response.data();

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
            }else if(obj.contains("nic"))
            {

                QJsonValue nics = obj.take("nic");
                nicList = nics.toArray();
                /**
                if(nics.isArray()){
                    nicList = nics.toArray();
                    for(int i = 0; i < nicList.count(); i++){
                        if(nicList[i].isObject()){
                            QJsonObject reportedDeviceObj = nicList[i].toObject().take("reported_devices").toObject();

                                if(reportedDeviceObj.take("reported_device").isArray()){


                                    QJsonArray reportedDeviceObjs = reportedDeviceObj.take("reported_device").toArray();
                                    //QJsonObject ips = reportedDeviceObjs[0].toObject().take("ips").toObject();
                                   // QJsonArray ip = ips.take("ip").toArray();



                                }
                            }
                        }
                    }
                }
                */

                //QJsonObject reported_devices = nicList[0].toObject();

                ipAddress = nicList[0].toObject().take("reported_devices").toObject().take("reported_device").toArray()[0].toObject().take("ips").toObject().
                        take("ip").toArray()[0].toObject().take("address").toString();

                //组装RDP连接文件并启动连接过程

                Utils::connectRDP(ipAddress, vmID);

                pIndicator->stopAnimation();

            }
        }
    }
}

void VMEle::spiceConnectSlot(HttpRequestWorker *worker)
{
    QJsonArray vmsList;
    //获取返回的所有VM的JSON描述信息
    char *response = worker->response.data();

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
            }else if(obj.contains("graphics_console"))
            {

                QJsonValue vms = obj.take("graphics_console");
                QString graphicsID;
                if(vms.isArray()){
                    graphicsID = vms.toArray().at(0).toObject().take("id").toString();
                }

                HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms/"+vmID+"/graphicsconsoles/" + graphicsID + "/remoteviewerconnectionfile", "POST");
                input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
                input->headers.insert("Accept", "application/json");

                input->request_body = QString("<action/>").toLatin1();

                worker = new HttpRequestWorker();
                connect(worker, &HttpRequestWorker::on_execution_finished, this, &VMEle::spiceConnect2Slot);
                worker->execute(input);

            }
        }
    }
}

void VMEle::spiceConnect2Slot(HttpRequestWorker *worker)
{
    QJsonArray vmsList;
    //获取返回的所有VM的JSON描述信息
    char *response = worker->response.data();

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(worker->response, &json_error);



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
            }else if(obj.contains("remote_viewer_connection_file"))
            {

                QJsonValue vms = obj.take("remote_viewer_connection_file");

                QString str = vms.toString();
                str.replace(QString("fullscreen=0"),QString("fullscreen=1"));
                QFile outFile(QApplication::applicationDirPath() + "/" + vmID+".vv");
                outFile.open(QIODevice::WriteOnly | QIODevice::Append);
                QTextStream ts(&outFile);
                ts << str << endl;

                QString connectStr = "\"C:/Program Files/VirtViewer v6.0-256/bin/remote-viewer.exe\" "  +QApplication::applicationDirPath() + "/" + vmID+".vv";
                QByteArray connectByteArray = connectStr.toLatin1();

                outFile.close();

                WinExec(connectByteArray.data(), SW_SHOW);

                pIndicator->stopAnimation();


            }
        }
    }
}
