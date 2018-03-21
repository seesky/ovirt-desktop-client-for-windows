#include "utils.h"
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QtCore/QtCore>
#include <QtXml/QDomElement>
#include <winsock2.h>
#include <QNetworkInterface>
#include <Iphlpapi.h>
#include <QSettings>
#include <QStringList>
#include <QApplication>
#include <QObject>
#include "httprequest.h"
#include <QObject>
#include <QTextStream>


Utils::Utils(QObject *parent) : QObject(parent)
{

}

Utils::~Utils()
{

}

//获取系统当前的分辨率
//void Utils::getScreenInfo(int &w, int &h)
QString Utils::getScreenInfo()
{
    /**
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    w = screenRect.width();
    h = screenRect.height();
    */

    QString screenRect;
    QSettings *configIniRead;
    QString url = QApplication::applicationDirPath() + "/setting.ini";

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        screenRect = configIniRead->value("/DisplaySetting/CurrentResolutionRatio").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return screenRect;
}

bool Utils::setScreenInfo(QString str)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/DisplaySetting/CurrentResolutionRatio",str);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

//获取当前系统支持的分辨率
QList<QString> Utils::getSupportResolutionRatio()
{
    QList<QString> supportResolutionRatio;
    QString *supportResolutionRatioStr;
    QSettings *configIniRead;
    /**

    //QFile file("D:/Qt/QtProject/untitled/setting/setting.xml");
    QFile file("setting.xml");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug("[QDebug]:[getSupportResolutionRatio]:配置文件打开失败！");
        return supportResolutionRatio;
    }

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
         qDebug("[QDebug]:[getSupportResolutionRatio]:DomDocuemnt创建失败！");
         file.close();
         return supportResolutionRatio;
    }

    QDomElement tree = domDocument.documentElement();
    //QDomElement firstItem = tree.firstChildElement();


    //QDomNodeList secondItemlist = firstItem.childNodes();
    QDomNodeList itemList = tree.childNodes();

    for(int i = 0; i < itemList.count(); i++){
        if(itemList.at(i).toElement().tagName() == "DisplaySetting"){
            QDomNodeList supportList = itemList.at(i).toElement().firstChild().firstChild().childNodes();
            for(int j = 0; j < supportList.count(); j++)
            {

                QString ele = supportList.at(j).toElement().text();
                supportResolutionRatio.append(ele);
                //qDebug("[DEBUG][getSupportResolutionRatio]SupportResolutionRatio:%s",qPrintable(ele));
            }
        }
    }


    file.close();
    */


    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        QString supportResolutionRatioStr = configIniRead->value("/DisplaySetting/SupportResolutionRatio").toString();

        QStringList supportResolutionRatioStrList = supportResolutionRatioStr.split("|");

        supportResolutionRatio = supportResolutionRatioStrList;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return supportResolutionRatio;
}

//获取IP地址获取方式
QString Utils::getNetworkIPAddressMode()
{
    QString ipAddressModel;
    QSettings *configIniRead;
    /**
    QFile file("setting.xml");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug("[QDebug]:[getSupportResolutionRatio]:配置文件打开失败！");
        return ipAddressModel;
    }

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
         qDebug("[QDebug]:[getSupportResolutionRatio]:DomDocuemnt创建失败！");
         file.close();
         return ipAddressModel;
    }

    QDomElement tree = domDocument.documentElement();
    //QDomElement firstItem = tree.firstChildElement();


    //QDomNodeList secondItemlist = firstItem.childNodes();
    QDomNodeList itemList = tree.childNodes();

    for(int i = 0; i < itemList.count(); i++){
        if(itemList.at(i).toElement().tagName() == "NetworkSetting"){
            QDomElement ipModeElement = itemList.at(i).toElement().firstChild().toElement();
            ipAddressModel = ipModeElement.text();
            //qDebug("[DEBUG][getNetworkIPAddressMode]:%s", qPrintable(ipModeElement.text()));
        }
    }

    file.close();
    */
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        ipAddressModel = configIniRead->value("/NetworkSetting/IpMode").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return ipAddressModel;
}

QString Utils::getHostIpByWinSock()
{
    QString IP;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
        ipAddressesList.at(i).toIPv4Address())
        {
            // 如果这个地址不是127.0.0.1，并且是IPv4的地址，就将其赋值给IP，并结束for循环
            IP = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果IP为空，则将其赋值为127.0.0.1
    if (IP.isEmpty())
        IP = QHostAddress(QHostAddress::LocalHost).toString();
    return IP;

}

QString Utils::getGateway()
{
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
        unsigned long stSize = sizeof(IP_ADAPTER_INFO);
        int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
        if (ERROR_BUFFER_OVERFLOW == nRel){
            // ERROR_BUFFER_OVERFLOW：内存空间不够
            // 释放原来的内存空间
            delete pIpAdapterInfo;
            // 重新申请内存空间用来存储所有网卡信息
            pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
            // 再次调用GetAdaptersInfo
            nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);
        }
        if (ERROR_SUCCESS == nRel){
            while (pIpAdapterInfo){
                switch(pIpAdapterInfo->Type){
                case MIB_IF_TYPE_OTHER:
                    break;
                case MIB_IF_TYPE_ETHERNET:
                    break;
                case MIB_IF_TYPE_TOKENRING:
                    break;
                case MIB_IF_TYPE_FDDI:
                    break;
                case MIB_IF_TYPE_PPP:
                    break;
                case MIB_IF_TYPE_LOOPBACK:
                    break;
                case MIB_IF_TYPE_SLIP:
                    break;
                default:
                    break;
                }
                for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
                    if (i < pIpAdapterInfo->AddressLength-1){
                    }
                    else{
                    }
                    // 多个网卡、多个IP
                    IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
                    do{
                        if(QString(pIpAddrString->IpAddress.String) == getHostIpByWinSock()){
                            return QString(pIpAdapterInfo->GatewayList.IpAddress.String);
                        }
                        pIpAddrString=pIpAddrString->Next;
                    } while (pIpAddrString);
                    pIpAdapterInfo = pIpAdapterInfo->Next;
            }

        }
        //释放内存空间
        if (pIpAdapterInfo){
            delete pIpAdapterInfo;
        }

        return "0.0.0.0";
}

QString Utils::getNetmask(QString ipAddress)
{
    QString ret;
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof (IP_ADAPTER_INFO));

    if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
    }

    if (NO_ERROR == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            if (ipAddress == QString::fromStdString(pAdapter->IpAddressList.IpAddress.String))
            {
                ret = QString::fromStdString(pAdapter->IpAddressList.IpMask.String);
                break;
            } // 在这里循环可以获取所有网卡信息
            pAdapter = pAdapter->Next;
        }
    }

    if (pAdapterInfo)
    {
        delete pAdapterInfo;
    }

    return ret;
}

QList<QString> Utils::getDNS()
{
    QList<QString> dnsList;

    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    ULONG stSize = sizeof(IP_ADAPTER_INFO);
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //获得其大小

    if (ERROR_BUFFER_OVERFLOW == nRel)                      //重新申请所需要的空间
    {
        delete pIpAdapterInfo;
        pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
        nRel=GetAdaptersInfo(pIpAdapterInfo, &stSize);
    }

    IP_PER_ADAPTER_INFO* pPerAdapt  = NULL;
    ULONG ulLen = 0;
    int err = GetPerAdapterInfo( pIpAdapterInfo->Index, pPerAdapt, &ulLen);


    if( err == ERROR_BUFFER_OVERFLOW ) {
        pPerAdapt = ( IP_PER_ADAPTER_INFO* ) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, ulLen);
        err = GetPerAdapterInfo( pIpAdapterInfo->Index, pPerAdapt, &ulLen );

        if( err == ERROR_SUCCESS ) {
            IP_ADDR_STRING* pNext = &( pPerAdapt->DnsServerList );
            if (pNext && strcmp(pNext->IpAddress.String, "") != 0)
            {
                dnsList.append(QString::fromStdString(pNext->IpAddress.String));
                if (pNext = pNext->Next)
                    dnsList.append(QString::fromStdString(pNext->IpAddress.String));
                }
            }

            HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPerAdapt);
        }

    return dnsList;
}

bool Utils::IsIPaddress(QString ip){
    QRegExp rx2("(//d+)(//.)(//d+)(//.)(//d+)(//.)(//d +)");
    int pos = rx2.indexIn(ip);
    if(pos>-1)
    {
        for(int i=0;i<4;i++)
        {
            if( rx2.cap(i*2+1).toInt()>=255 )
            {

                return false;
            }
        }

        if(rx2.cap(7).toInt()==0)
        {

            return false;
        }

        if(rx2.cap(7).toInt()==0)
        {

            return false;
        }
    }
    else
    {

        return false;
    }

    return true;
}

bool Utils::setIPMode(QString str){

    /**
    QFile file("setting.xml");

    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        qDebug("[QDebug]:[getSupportResolutionRatio]:配置文件打开失败！");
        return false;
    }

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true)) {
         qDebug("[QDebug]:[getSupportResolutionRatio]:DomDocuemnt创建失败！");
         file.close();
         return false;
    }

    QDomElement tree = domDocument.documentElement();

    QDomNodeList itemList = tree.childNodes();

    for(int i = 0; i < itemList.count(); i++){
        if(itemList.at(i).toElement().tagName() == "NetworkSetting"){
            itemList.at(i).toElement().firstChild().firstChild().setNodeValue(str);
        }
    }

    file.close();

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug("[QDebug]:[getSupportResolutionRatio]:配置文件打开失败！");
        return false;
    }

    QTextStream ts(&file);
    ts.reset();
    ts.setCodec("utf-8");
    domDocument.save(ts, 4, QDomNode::EncodingFromTextStream);
    file.close();
    */

    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->value("/NetworkSetting/IpMode").toString();
        configIniRead->setValue("/NetworkSetting/IpMode",str);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }


}

bool Utils::setStaticIpAddress(QString ipaddress, QString netmask, QString gateway, QString dns1, QString dns2){

}

QString Utils::getServerIPAddress()
{
    QString serverIPAddress;
    QSettings *configIniRead;

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        serverIPAddress = configIniRead->value("/ServerSetting/ServerIpAddress").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return serverIPAddress;
}

bool Utils::setServerIPAddress(QString str)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/ServerSetting/ServerIpAddress",str);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

bool Utils::setCurrentToken(QString str)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/ServerSetting/CurrentToken",str);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

QString Utils::getCurrentToken()
{
    QString currentToken;
    QSettings *configIniRead;

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        currentToken = configIniRead->value("/ServerSetting/CurrentToken").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return currentToken;
}

QString Utils::getConnectMode()
{
    QString connectMode;

    QSettings *configIniRead;

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        connectMode = configIniRead->value("/ConnectModeSetting/mode").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return connectMode;
}

bool Utils::setConnectMode(QString str)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/ConnectModeSetting/mode",str);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

void Utils::getVMID()
{

    //先从可用池中申请虚拟机
    allocateVMInput = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vmpools", "GET");
    allocateVMInput->headers.insert("Accept", "application/xml");
    allocateVMInput->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    allocateVMInput->headers.insert("Accept", "application/json");

    allocateVMWorker = new HttpRequestWorker();
    connect(allocateVMWorker, &HttpRequestWorker::on_execution_finished, this, &Utils::allocateVMFromPoolSolt);
    allocateVMWorker->execute(allocateVMInput);


    input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vms", "GET");
    //input = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vmpools", "GET");
    input->headers.insert("Accept", "application/xml");
    input->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
    input->headers.insert("Accept", "application/json");

    //////////////////////////
    //input->headers.insert("Filter", "true");

    worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &Utils::getVMIDSlot);
    worker->execute(input);
}

void Utils::allocateVMFromPoolSolt(HttpRequestWorker *worker)
{
    QJsonArray vmsList;
    QJsonArray vmPoolsList;
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
            }else if(obj.contains("vm_pool"))
            {
                QJsonValue vmpools = obj.take("vm_pool");


                if(vmpools.isArray()){
                    vmPoolsList = vmpools.toArray();
                    QList<QString> vmReturn;
                    QHash<QString, QString> vmsHash;
                    for(int j = 0; j < vmPoolsList.count(); j++){
                        if(vmPoolsList.at(j).isObject()){
                            //QJsonValue vms = vmpools.toObject().take("vm");
                            //QJsonValue vms = vmPoolsList.at(j).toObject().take("vm");

                            HttpRequestInput *tmpInput = new HttpRequestInput("https://" + Utils::getServerIPAddress() + "/ovirt-engine/api/vmpools/" + vmPoolsList.at(j).toObject().take("id").toString() + "/allocatevm", "POST");
                            tmpInput->headers.insert("Accept", "application/xml");
                            tmpInput->headers.insert("Authorization", "Bearer " + Utils::getCurrentToken());
                            tmpInput->headers.insert("Accept", "application/json");
                            tmpInput->request_body = QString("<action/>").toLatin1();


                            HttpRequestWorker *tmpWorker = new HttpRequestWorker();

                            tmpWorker->execute(tmpInput);
                        }
                    }

                }
             }






            }
        }
}

void Utils::getVMIDSlot(HttpRequestWorker *worker)
{
    QJsonArray vmsList;
    QJsonArray vmPoolsList;
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
            }else if(obj.contains("vm")) //else if(obj.contains("vm_pool"))
            {
                QJsonValue vms = obj.take("vm");

                if(vms.isArray()){
                    vmsList = vms.toArray();
                    QList<QString> vmReturn;
                    QHash<QString, QString> vmsHash;


                                for(int i = 0; i < vmsList.count(); i++){

                                    vmReturn.append(vmsList.at(i).toObject().take("id").toString());

                                    vmsHash.insert(vmsList.at(i).toObject().take("id").toString(), vmsList.at(i).toObject().take("name").toString());
                                }

                                emit getVMIDSignal(vmsHash);


               }

             }


                /**
                QJsonValue access_token = obj.take("access_token");
                if(access_token.isString())
                {
                    QString _access_token = access_token.toString();
                    Utils::setCurrentToken(_access_token);

                    ui->usernamelineEdit->setText("");
                    ui->passwordlineEdit->setText("");
                    restAPIResultLabel->setText("");

                    seesionStatusUpdate = new SessionStatusUpdateThread();
                    connect(seesionStatusUpdate, SIGNAL(sessionStatusUpdate(bool)), this, SLOT(sessionStatusUpdateSolts(bool)),Qt::DirectConnection);
                    seesionStatusUpdate->start();

                    //登陆成功
                    vmListWindow = new VMListWindow();
                    vmListWindow->show();

                }
                */


                /**
                //QJsonValue vms = obj.take("vm");
                QJsonValue vmpools = obj.take("vm_pool");


                if(vmpools.isArray()){
                    vmPoolsList = vmpools.toArray();
                    QList<QString> vmReturn;
                    QHash<QString, QString> vmsHash;
                    for(int j = 0; j < vmPoolsList.count(); j++){
                        if(vmPoolsList.at(j).isObject()){
                            //QJsonValue vms = vmpools.toObject().take("vm");
                            QJsonValue vms = vmPoolsList.at(j).toObject().take("vm");


                            if(vms.isArray()){
                                vmsList = vms.toArray();

                                for(int i = 0; i < vmsList.count(); i++){

                                    vmReturn.append(vmsList.at(i).toObject().take("id").toString());

                                    vmsHash.insert(vmsList.at(i).toObject().take("id").toString(), vmsList.at(i).toObject().take("name").toString());
                                }

                            }else if(vms.isObject()){

                                vmReturn.append(vms.toObject().take("id").toString());
                                vmsHash.insert(vms.toObject().take("id").toString(), vms.toObject().take("name").toString());
                            }
                        }
                    }
                    emit getVMIDSignal(vmsHash);
                }
                */

                   /**
                if(vmpools.isObject()){
                    QJsonValue vms = vmpools.toObject().take("vm");


                    if(vms.isArray()){
                        vmsList = vms.toArray();
                        QList<QString> vmReturn;
                        QHash<QString, QString> vmsHash;
                        for(int i = 0; i < vmsList.count(); i++){

                            vmReturn.append(vmsList.at(i).toObject().take("id").toString());

                            vmsHash.insert(vmsList.at(i).toObject().take("id").toString(), vmsList.at(i).toObject().take("name").toString());
                        }
                        emit getVMIDSignal(vmsHash);
                    }else if(vms.isObject()){
                        QList<QString> vmReturn;
                        QHash<QString, QString> vmsHash;
                        vmReturn.append(vms.toObject().take("id").toString());
                        vmsHash.insert(vms.toObject().take("id").toString(), vms.toObject().take("name").toString());
                        emit getVMIDSignal(vmsHash);
                    }
                }
                */


            }
        }
    }


void Utils::connectRDP(QString ipAddress, QString vmID)
{

    QFile file1(QApplication::applicationDirPath() + "/"+ vmID +".rdp");
    if (file1.exists())
    {
        file1.remove();
    }
    file1.close();

    QFile::copy(QApplication::applicationDirPath() + "/rdpTemplate.rdp", QApplication::applicationDirPath() + "/"+ vmID +".rdp");

    if(QFile(QApplication::applicationDirPath() + "/"+ vmID +".rdp").exists())
    {
        QFile file(QApplication::applicationDirPath() + "/"+ vmID +".rdp");
        file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
        QTextStream in(&file);
        //设置连接文件中的IP地址
        QString ipStr = "full address:s:"+ipAddress;
        in<<ipStr<<"\n";
        //设置连接文件中的屏幕尺寸
        QString screenInfoTmp = Utils::getScreenInfo();
        QString screenWidth = "desktopwidth:i:" + screenInfoTmp.section("*",0,0);
        QString screenHeight = "desktopheight:i:" + screenInfoTmp.section("*",1,1);
        in<<screenWidth<<"\n";
        in<<screenHeight<<"\n";

        file.close();
        //启动连接程序
        QString connectStr = "\"mstsc\" "  +QApplication::applicationDirPath() + "/" + vmID+".rdp";
        QByteArray connectByteArray = connectStr.toLatin1();

        WinExec(connectByteArray.data(), SW_SHOW);

    }else{
        qDebug("[DEBUG][RDP连接]临时连接文件不存在");
    }
}

QString Utils::getDefaultUsername()
{
    QString defaultUsername;
    QSettings *configIniRead;
    QString url = QApplication::applicationDirPath() + "/setting.ini";

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        defaultUsername = configIniRead->value("/DefaultUsername/username").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return defaultUsername;
}

bool Utils::setDefaultUsername(QString username)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/DefaultUsername/username",username);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

QString Utils::getDefaultPassword()
{
    QString defaultPassword;
    QSettings *configIniRead;
    QString url = QApplication::applicationDirPath() + "/setting.ini";

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        defaultPassword = configIniRead->value("/DefaultUsername/password").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return defaultPassword;
}

bool Utils::setDefaultPassword(QString password)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/DefaultUsername/password",password);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}

QString Utils::getEnableUsername()
{
    QString enableUsername;
    QSettings *configIniRead;
    QString url = QApplication::applicationDirPath() + "/setting.ini";

    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        enableUsername = configIniRead->value("/DefaultUsername/enable").toString();
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
    }

    return enableUsername;
}

bool Utils::setEnableUsername(QString enable)
{
    QSettings *configIniRead;
    if(QFile(QApplication::applicationDirPath() + "/setting.ini").exists())
    {
        configIniRead = new QSettings(QApplication::applicationDirPath()+"/setting.ini", QSettings::IniFormat);
        configIniRead->setValue("/DefaultUsername/enable",enable);
        return true;
    }else{
        qDebug("[DEBUG][getSupportResolutionRatio]配置文件不存在");
        return false;
    }
}



