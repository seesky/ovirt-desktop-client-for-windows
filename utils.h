#ifndef UTILS_H
#define UTILS_H

#include <QList>
#include "httprequest.h"
#include <QObject>


class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);
    ~Utils();
    //获取系统当前分辨率
    //static void getScreenInfo(int &w, int &h);
    static QString getScreenInfo();
    static bool setScreenInfo(QString str);
    static QList<QString> getSupportResolutionRatio();
    static QString getNetworkIPAddressMode();
    static QString getHostIpByWinSock();
    static QString getGateway();
    static QString getNetmask(QString ipAddress);
    static QList<QString> getDNS();
    static bool IsIPaddress(QString ip);
    static bool setIPMode(QString str);
    static bool setStaticIpAddress(QString ipaddress, QString netmask, QString gateway, QString dns1="114.114.114.114", QString dns2="8.8.8.8");
    static QString getServerIPAddress();
    static bool setServerIPAddress(QString str);
    static bool setCurrentToken(QString str);
    static QString getCurrentToken();
    static QString getConnectMode();
    static bool setConnectMode(QString str);
    static void connectRDP(QString ipAddress, QString vmID);
    void getVMID();
    static QString getDefaultUsername();
    static bool setDefaultUsername(QString username);
    static QString getDefaultPassword();
    static bool setDefaultPassword(QString password);
    static QString getEnableUsername();
    static bool setEnableUsername(QString enable);


private:
    HttpRequestInput *input;
    HttpRequestWorker *worker;
    QList<QString> *vmList;
    HttpRequestInput *allocateVMInput;
    HttpRequestWorker *allocateVMWorker;




public slots:
    void getVMIDSlot(HttpRequestWorker *worker);
    void allocateVMFromPoolSolt(HttpRequestWorker *worker);


signals:
    void getVMIDSignal(QHash<QString, QString>);

};

#endif // UTILS_H
