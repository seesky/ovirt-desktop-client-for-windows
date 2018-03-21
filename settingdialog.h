#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QFile>
#include <QtNetwork>



namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

signals:
    void resetScreenSize();

private slots:
    void on_setResolutionRatioButton_clicked();

    void on_networkModeManualRadioButton_clicked();
    
    void on_networkModeAutoRadioButton_clicked();
    
    void on_refreshIpShowPushButton_clicked();

    void on_ipAddressLineEdit_textChanged(const QString &arg1);

    void on_setIPAddressModePushButton_clicked();

    void showPingResult(QString str);

    void showSetIPAddressResult(QString str);

    void on_pingPushButton_clicked();

    void on_regetIPPushButton_clicked();

    void on_saveServerIPAddresspushButton_clicked();

    void on_saveConnectModePushButton_clicked();



    void on_flushKeyPushButton_clicked();

    void on_saveDefaultUsernamePushButton_clicked();

    void on_enableDefaultUsernameRadioButton_clicked();

public slots:
    void onServerKeyDownloadFinished(QNetworkReply *reply);
    void serverKeyDownloadProgress(qint64 a, qint64 b);

private:
    Ui::SettingDialog *ui;

    QFile * serverKeyFile;
    QNetworkReply * serverKeyReply;
    QNetworkAccessManager * serverKeyManager;
};

#endif // SETTINGDIALOG_H
