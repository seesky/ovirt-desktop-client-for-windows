#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "utils.h"
#include <QList>
#include "windows.h"
#include <QRegExp>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include "pingthread.h"
#include "execcmd.h"
#include <QFileInfo>
#include "QMessageBox"

int screenWidth;
int screenHeight;
QList<QString> supportResolutionRatioList;
QString ipMode;
QString ipAddress;
QString connectMode;
QList<QString> dnsList;
PingThread *pingThread;

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    //设置cmd运行结构显示textedit不可编辑
    ui->ResultTextEdit->setReadOnly(true);

    ui->defaultPasswordEdit->setEchoMode(QLineEdit::Password);

    //获取当前分辨率
    //Utils::(screenWidth,screenHeight);
    //ui->currentResolutionRatio->setText(QString::number(screenWidth) + " X " + QString::number(screenHeight));
    ui->currentResolutionRatio->setText(Utils::getScreenInfo());

    //获取系统当前支持的分辨率
    supportResolutionRatioList = Utils::getSupportResolutionRatio();
    for(int i = 0; i < supportResolutionRatioList.count(); i++){
        ui->supportResolutionComboBox->addItem(supportResolutionRatioList[i]);
    }

    //获取当前的连接模式
    connectMode = Utils::getConnectMode();
    if(connectMode == "rdp"){
        ui->rdpConnectRadioButton->setChecked(true);
        ui->spiceConnectRadioButton->setChecked(false);
    }else if(connectMode == "spice"){
        ui->rdpConnectRadioButton->setChecked(false);
        ui->spiceConnectRadioButton->setChecked(true);
    }

    //获取当前IP地址的获取方式，并更新界面，ipMode为1则为手动模式
    ipMode = Utils::getNetworkIPAddressMode();
    if(ipMode == "1"){
        ui->networkModeManualRadioButton->setChecked(true);
        ui->networkModeAutoRadioButton->setChecked(false);
        ui->regetIPPushButton->setEnabled(false);
    }else{
        ui->networkModeAutoRadioButton->setChecked(true);
        ui->networkModeManualRadioButton->setChecked(false);
        ui->ipAddressLineEdit->setEnabled(false);
        ui->netmaskLineEdit->setEnabled(false);
        ui->gatewayLineEdit->setEnabled(false);
        ui->dns1LineEdit->setEnabled(false);
        ui->dns2LineEdit->setEnabled(false);
    }

    //获取当前的IP地址，如果是手动配置则直接显示，自动获取如果刚开始获取不到，可以点击刷新重新加载
    ipAddress = Utils::getHostIpByWinSock();
    ui->ipAddressLineEdit->setText(ipAddress);
    ui->netmaskLineEdit->setText(Utils::getNetmask(ipAddress));
    ui->gatewayLineEdit->setText(Utils::getGateway());
    dnsList = Utils::getDNS();
    if(dnsList.count() > 0)
    {
        ui->dns1LineEdit->setText(dnsList.at(0));
    }

    if(dnsList.count() > 1){
        ui->dns2LineEdit->setText(dnsList.at(1));
    }


    //检测IP、NETMASK、GATWAY、DNS的格式是否正确，不正确不允许输入
    QRegExp regExpIP("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    QRegExp regExpNetPort("((6553[0-5])|[655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{3}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])");
    QRegExp regExpChannel("[0-5]");
    ui->ipAddressLineEdit->setValidator(new QRegExpValidator(regExpIP,this));
    ui->netmaskLineEdit->setValidator(new QRegExpValidator(regExpIP,this));
    ui->gatewayLineEdit->setValidator(new QRegExpValidator(regExpIP,this));
    ui->dns1LineEdit->setValidator(new QRegExpValidator(regExpIP,this));
    ui->dns2LineEdit->setValidator(new QRegExpValidator(regExpIP,this));
    ui->pingTagetIPAddresslineEdit->setValidator(new QRegExpValidator(regExpIP,this));

    //加载当前服务器IP
    ui->serverIPAddresslineEdit->setText(Utils::getServerIPAddress());
    //ui->serverIPAddresslineEdit->setValidator(new QRegExpValidator(regExpIP,this));

    serverKeyManager = new QNetworkAccessManager(this);
    connect(serverKeyManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onServerKeyDownloadFinished(QNetworkReply*)));

    //隐藏刷新按钮
    ui->flushKeyPushButton->setVisible(false);
    ui->progressBar->setVisible(false);

    //加载默认用户名密码
    ui->defaultUsernameEdit->setText(Utils::getDefaultUsername());
    ui->defaultPasswordEdit->setText(Utils::getDefaultPassword());
    if(Utils::getEnableUsername() == "1")
    {
        ui->enableDefaultUsernameCheckBox->setChecked(Qt::Checked);
    }else{
        ui->enableDefaultUsernameCheckBox->setChecked(Qt::Unchecked);
    }
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

//设置系统分辨率，并向主窗口发送尺寸更新信号
void SettingDialog::on_setResolutionRatioButton_clicked()
{
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
    DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    //qDebug("[QDEBUG][on_setResolutionRationButton_clicked]:%s", qPrintable(ui->supportResolutionComboBox->currentText().section("*",0,0)));
    DevMode.dmPelsWidth =  ui->supportResolutionComboBox->currentText().section("*",0,0).toInt();
    DevMode.dmPelsHeight = ui->supportResolutionComboBox->currentText().section("*",1,1).toInt();
    //ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
    long ok = ChangeDisplaySettings(&DevMode, CDS_UPDATEREGISTRY);
    Sleep(2000);

    if(Utils::setScreenInfo(ui->supportResolutionComboBox->currentText()))
    {
        ui->currentResolutionRatio->setText(ui->supportResolutionComboBox->currentText());
    }

    if(ok == DISP_CHANGE_SUCCESSFUL)
    {
        emit resetScreenSize();
    }





}

void SettingDialog::on_networkModeManualRadioButton_clicked()
{
    ui->ipAddressLineEdit->setEnabled(true);
    ui->netmaskLineEdit->setEnabled(true);
    ui->gatewayLineEdit->setEnabled(true);
    ui->dns1LineEdit->setEnabled(true);
    ui->dns2LineEdit->setEnabled(true);
    ipMode = "1";
}

//自动获取IP
void SettingDialog::on_networkModeAutoRadioButton_clicked()
{
    ui->ipAddressLineEdit->setEnabled(false);
    ui->netmaskLineEdit->setEnabled(false);
    ui->gatewayLineEdit->setEnabled(false);
    ui->dns1LineEdit->setEnabled(false);
    ui->dns2LineEdit->setEnabled(false);
    ui->regetIPPushButton->setEnabled(true);
    ipMode = "0";
}

//重新过去本地IP地址并显示
void SettingDialog::on_refreshIpShowPushButton_clicked()
{
    //获取当前的IP地址，如果是手动配置则直接显示，自动获取如果刚开始获取不到，可以点击刷新重新加载
    ipAddress = Utils::getHostIpByWinSock();
    ui->ipAddressLineEdit->setText(ipAddress);
    ui->netmaskLineEdit->setText(Utils::getNetmask(ipAddress));
    ui->gatewayLineEdit->setText(Utils::getGateway());
    dnsList = Utils::getDNS();
    if(dnsList.count() > 0)
    {
        ui->dns1LineEdit->setText(dnsList.at(0));
    }

    if(dnsList.count() > 1){
         ui->dns2LineEdit->setText(dnsList.at(1));
    }
}

void SettingDialog::on_ipAddressLineEdit_textChanged(const QString &arg1)
{

}

void SettingDialog::on_setIPAddressModePushButton_clicked()
{
    ui->setIPAddressModePushButton->setEnabled(false);

    if(ipMode == "1")
    {
        //手动设置IP地址
        if(ui->ipAddressLineEdit->text().isEmpty() || ui->netmaskLineEdit->text().isEmpty() || ui->gatewayLineEdit->text().isEmpty()){
            QMessageBox::about(NULL, "输入错误", "请填写完整的IP地址、子网掩码和网管");
        }else{
            //执行cmd命令配置IP地址
            if(Utils::setIPMode("1"))
            {
                //设置静态IP地址
                QString cmdIPStr = "netsh interface ipv4 set address name=\"本地连接\" source=static address="+ ui->ipAddressLineEdit->text() +" mask="+ ui->netmaskLineEdit->text() +" gateway="+ ui->gatewayLineEdit->text() +" gwmetric=0";
                ExecCMD *setIPAddressThread = new ExecCMD(cmdIPStr);
                connect(setIPAddressThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
                setIPAddressThread->start();
                if(setIPAddressThread->wait())
                {
                    ui->ResultTextEdit->setText("保存静态IP地址完成！" + ui->ResultTextEdit->toPlainText());


                        //设置静态DNS地址

                        if(ui->dns1LineEdit->text() != "")
                        {
                            QString cmdDNS1StaticStr = "netsh interface ipv4 set dns name=\"本地连接\" source=static address="+ ui->dns1LineEdit->text();
                            ExecCMD *setDNS1StaticThread = new ExecCMD(cmdDNS1StaticStr);
                            connect(setDNS1StaticThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
                            setDNS1StaticThread->start();
                            if(setDNS1StaticThread->wait())
                            {
                                if(ui->dns2LineEdit->text() != "")
                                {
                                    QString cmdDNS2StaticStr = "netsh interface ipv4 add dnsserver \"本地连接\" " + ui->dns2LineEdit->text() + " index=2";
                                    ExecCMD *setDNS2StaticThread = new ExecCMD(cmdDNS2StaticStr);
                                    connect(setDNS2StaticThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
                                    setDNS2StaticThread->start();
                                    ui->setIPAddressModePushButton->setEnabled(true);

                                }
                            }

                        }else if((ui->dns2LineEdit->text() != "") && (ui->dns1LineEdit->text() == "")){
                            QString cmdDNS2StaticStr = "netsh interface ipv4 add dnsserver \"本地连接\" " + ui->dns2LineEdit->text() + " index=2";
                            ExecCMD *setDNS2StaticThread = new ExecCMD(cmdDNS2StaticStr);
                            connect(setDNS2StaticThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
                            setDNS2StaticThread->start();
                            ui->setIPAddressModePushButton->setEnabled(true);
                        }


                }





            }
        }
    }else{
        if(Utils::setIPMode("0"))
        {
            //自动获取IP地址：先设置为DHCP模式，然后ipconfig /new
            QString cmdIPStr = "netsh interface ip set address name=\"本地连接\" source=dhcp";
            QString cmdDNSDhcpStr = "netsh interface ip set dns name=\"本地连接\" source=dhcp ";

            ExecCMD *setIPAddressDHCPThread = new ExecCMD(cmdIPStr);
            connect(setIPAddressDHCPThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
            setIPAddressDHCPThread->start();
            if(setIPAddressDHCPThread->wait())
            {
                ExecCMD *setDNSDhcpThread = new ExecCMD(cmdDNSDhcpStr);
                connect(setDNSDhcpThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
                setDNSDhcpThread->start();
            }




        }
    }
}

//配置IP地址命令返回处理槽
void SettingDialog::showSetIPAddressResult(QString str)
{
    ui->ResultTextEdit->setText(str + ui->ResultTextEdit->toPlainText());

}


void SettingDialog::showPingResult(QString str){
    ui->ResultTextEdit->setText(str + ui->ResultTextEdit->toPlainText());
    ui->pingPushButton->setEnabled(true);
}

void SettingDialog::on_pingPushButton_clicked()
{
    QString pingIPAddress = ui->pingTagetIPAddresslineEdit->text();
    /**
    QProcess p(0);
    p.start("ping " + pingIPAddress);
    p.waitForStarted();
    p.waitForFinished();

    QString out = QString::fromLocal8Bit(p.readAllStandardOutput());

    ui->ResultTextEdit->setText(out);
    */


    /**
    pingThread = new PingThread(pingIPAddress);
    //connect(PingThread, SIGNAL(pingResult(QString)), this, SLOT(showPingResult(QString)),Qt::DirectConnection);
    connect(pingThread, SIGNAL(pingResult(QString)), this, SLOT(showPingResult(QString)));
    pingThread->start();
    ui->pingPushButton->setEnabled(false);
    */

    ExecCMD * pingThread = new ExecCMD("ping "+pingIPAddress);
    connect(pingThread, SIGNAL(exec(QString)), this, SLOT(showPingResult(QString)));
    pingThread->start();
    ui->pingPushButton->setEnabled(false);

}

void SettingDialog::on_regetIPPushButton_clicked()
{
    ui->regetIPPushButton->setEnabled(false);
    QString cmdRefreshIP1 = "ipconfig /release";
    QString cmdRefreshIP2 = "ipconfig /renew";

    ExecCMD *releaseIPAddressThread = new ExecCMD(cmdRefreshIP1);
    connect(releaseIPAddressThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
    releaseIPAddressThread->start();
    if(releaseIPAddressThread->wait())
    {
        ExecCMD *renewIPAddressThread = new ExecCMD(cmdRefreshIP2);
        connect(renewIPAddressThread, SIGNAL(exec(QString)), this, SLOT(showSetIPAddressResult(QString)));
        renewIPAddressThread->start();
        ui->regetIPPushButton->setEnabled(true);
    }
}

void SettingDialog::on_saveServerIPAddresspushButton_clicked()
{
    Utils::setServerIPAddress(ui->serverIPAddresslineEdit->text());
    ui->ResultTextEdit->setText("服务器IP地址设置为:"+ ui->serverIPAddresslineEdit->text() + "\n" + ui->ResultTextEdit->toPlainText());
}

//设置连接模式保存到配置文件里
void SettingDialog::on_saveConnectModePushButton_clicked()
{
    if(ui->rdpConnectRadioButton->isChecked()){
        if(Utils::setConnectMode("rdp")){
            ui->ResultTextEdit->setText("连接模式设置为:RDP模式\n" + ui->ResultTextEdit->toPlainText());
        }
    }else{
        if(Utils::setConnectMode("spice")){
            ui->ResultTextEdit->setText("连接模式设置为:SPICE模式\n" + ui->ResultTextEdit->toPlainText());
        }
    }
}

void SettingDialog::serverKeyDownloadProgress(qint64 a, qint64 b)
{
    serverKeyFile->write(serverKeyReply->readAll());
    QByteArray test = serverKeyReply->readAll();
    ui->progressBar->setMaximum(b);
    ui->progressBar->setValue(a);
}

void SettingDialog::onServerKeyDownloadFinished(QNetworkReply * reply)//reply用于接收消息
{
    //serverKeyFile->write(serverKeyReply->readAll());
    qDebug() << "Your thing had finished downloading.";
    serverKeyFile->flush();
    serverKeyFile->close();
    reply->deleteLater();

}



void SettingDialog::on_flushKeyPushButton_clicked()
{
    //删除原文件
    //QFile file(QApplication::applicationDirPath()+"/pki-resource.cer");
    //file.remove();
    //file.close();

    //下载并创建新文件
    //QFileInfo info(QApplication::applicationDirPath()+"/pki-resource.cer");
    serverKeyFile = new QFile(QApplication::applicationDirPath()+"/pki-resource.cer");
    serverKeyFile->open(QIODevice::WriteOnly | QIODevice::Text);
    QString url = "https://" + Utils::getServerIPAddress() + "/ovirt-engine/services/pki-resource?resource=ca-certificate&format=X509-PEM-CA";
    qDebug(qPrintable(url));
    serverKeyReply = serverKeyManager->get(QNetworkRequest(QUrl(url)));
    connect(serverKeyReply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(serverKeyDownloadProgress(qint64,qint64)));
}

void SettingDialog::on_saveDefaultUsernamePushButton_clicked()
{

    if(ui->enableDefaultUsernameCheckBox->isChecked())
    {
        if(ui->defaultUsernameEdit->text() != "" && ui->defaultPasswordEdit->text() != "")
        {
            Utils::setEnableUsername("1");
            Utils::setDefaultUsername(ui->defaultUsernameEdit->text());
            Utils::setDefaultPassword(ui->defaultPasswordEdit->text());
            ui->ResultTextEdit->setText("保存默认用户名密码完成！" + ui->ResultTextEdit->toPlainText());
        }else{
            QMessageBox::about(NULL, "输入错误", "用户名或密码不能为空");
        }

    }else{
        Utils::setEnableUsername("0");
    }


}

void SettingDialog::on_enableDefaultUsernameRadioButton_clicked()
{

}
