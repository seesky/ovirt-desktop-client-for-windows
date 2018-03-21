#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QCloseEvent"
#include <QLabel>
#include <QTimer>
#include <QString>
#include "QMovie"
#include "settingdialog.h"
#include "statusupdatethread.h"
#include "windows.h"
#include "httprequest.h"
#include "utils.h"
#include "qprogressindicator.h"
#include <QJsonDocument>
#include "sessionstatusupdatethread.h"
#include "vmlistwindow.h"
#include "windows.h"
#include "QMessageBox"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    currentTimeLabel = new QLabel("");
    currentTimeLabel1 = new QLabel("");
    //设置状态栏颜色
    statusBar()->setStyleSheet("background-color:rgba(244,244,244,2);color: rgb(203, 203, 203);");


    //登陆等待动画
    pIndicator = new QProgressIndicator(this);
    pIndicator->setColor(Qt::white);

    //设置主窗口颜色
    this->setObjectName("MainWindow");
    this->setStyleSheet("#MainWindow{border-image:url(:/image/image/backgroud.jpg);}");

    //设置密码输入框样式
    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);

    //启动状态检测线程，实时跟新系统状态
    statusUpdateThread = new StatusUpdateThread();
    connect(statusUpdateThread, SIGNAL(updateStatus(QString)), this, SLOT(statusUpdate(QString)),Qt::DirectConnection);
    statusUpdateThread->start();
    currentTimeLabel->setStyleSheet("color:white");
    ui->statusBar->addWidget(currentTimeLabel);

    //访问restapi的错误返回信息
    restAPIResultLabel = new QLabel();
    restAPIResultLabel->setStyleSheet("color:white");
    ui->statusBar->addWidget(restAPIResultLabel);

    settingDialog = new SettingDialog();

    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);

    showFullScreen();

    bool bOldState = false;
    SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, &bOldState, 0);

    //加载默认用户名密码

    if(Utils::getEnableUsername() == "1")
    {
        ui->usernamelineEdit->setText(Utils::getDefaultUsername());
        ui->passwordlineEdit->setText(Utils::getDefaultPassword());
        ui->usernamelineEdit->setEnabled(false);
        ui->passwordlineEdit->setEnabled(false);
    }else{
        ui->usernamelineEdit->setEnabled(true);
        ui->passwordlineEdit->setEnabled(true);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_2_clicked()
{
    showFullScreen();
}

void MainWindow::on_pushButton_3_clicked()
{
    showNormal();
}


void MainWindow::closeEvent(QCloseEvent *event)
{

     event->ignore();

}


void MainWindow::on_settingButton_clicked()
{

    settingDialog->setModal(true);
    //settingDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    settingDialog->show();
    settingDialog->setVisible(false);
    settingDialog->setVisible(true);

    //绑定分辨率修改信号，更新客户端当前尺寸
    connect(settingDialog, SIGNAL(resetScreenSize()), this, SLOT(resetScreentSize()));
}

void MainWindow::statusUpdate(QString str){
    currentTimeLabel->setText(str);
}

//根据当前设置刷新主窗口尺寸
void MainWindow::resetScreentSize()
{
    //showNormal();
    //Sleep(2000);
    showFullScreen();
}



void MainWindow::on_loginPushButton_clicked()
{

    HttpRequestInput *input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/sso/oauth/token", "POST");
    input->add_var("grant_type", "password");
    input->add_var("scope", "ovirt-app-api");
    QString username = ui->usernamelineEdit->text().trimmed();
    QString password = ui->passwordlineEdit->text().trimmed();
    input->add_var("username", username + "@internal");
    input->add_var("password", password);
    //input->headers.insert("Authorization", "Basic " + QByteArray(QString("%1:%2").arg("admin%40internal").arg("hstecs123456").toLatin1()).toBase64());
    input->headers.insert("Accept", "application/json");

    HttpRequestWorker *worker = new HttpRequestWorker();
    //connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker *workers)), this, SLOT(http_request_result(HttpRequestWorker *workers)));
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &MainWindow::http_request_result);
    worker->execute(input);
    pIndicator->startAnimation();
}


void MainWindow::http_request_result(HttpRequestWorker *worker)
{
    char *response = worker->response.data();

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(worker->response, &json_error);

    //qDebug(response);

    if(json_error.error == QJsonParseError::NoError)
    {

        if(parse_doucment.isObject())
        {
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
                    QMessageBox::about(NULL, "登录失败，请检查您的账号密码是否正确！", "  错误代码：" + _errorCode + "  错误内容：" + _error);
                }
            }else if(obj.contains("access_token"))
            {
                QJsonValue access_token = obj.take("access_token");
                if(access_token.isString())
                {
                    QString _access_token = access_token.toString();
                    Utils::setCurrentToken(_access_token);

                    if(Utils::getEnableUsername() == "1"){

                    }else{
                        ui->usernamelineEdit->setText("");
                        ui->passwordlineEdit->setText("");
                        restAPIResultLabel->setText("");
                    }


                    //5分钟后自动退出主界面回到登陆界面
                    seesionStatusUpdate = new SessionStatusUpdateThread();
                    connect(seesionStatusUpdate, SIGNAL(sessionStatusUpdate(bool)), this, SLOT(sessionStatusUpdateSolts(bool)),Qt::DirectConnection);
                    seesionStatusUpdate->start();


                    //登陆成功
                    vmListWindow = new VMListWindow();

                    connect(vmListWindow, VMListWindow::sessionExit, this, MainWindow::sessionExit);

                    vmListWindow->show();
                    this->hide();


                }
            }
        }
    }

    pIndicator->stopAnimation();
}

void MainWindow::sessionExit(bool sessionExitMode)
{
    if(sessionExitMode == true){
        this->show();
        seesionStatusUpdate->quit();
    }
}

void MainWindow::sessionStatusUpdateSolts(bool valid)
{
    if(valid == true)
    {
        //qDebug("Session有效");

    }else{
        //qDebug("Session无效");
        this->show();
        vmListWindow->close();
        this->seesionStatusUpdate->quit();
    }

}
