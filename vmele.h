#ifndef VMELE_H
#define VMELE_H

#include <QWidget>
#include "vmstatusupdatethread.h"
#include <QString>
#include "httprequest.h"
#include "qprogressindicator.h"

namespace Ui {
class VMEle;
}

class VMEle : public QWidget
{
    Q_OBJECT

public:
    explicit VMEle(QWidget *parent = 0);
    ~VMEle();
    QString vmID;
    QString vmName;
    HttpRequestWorker *worker;
    QProgressIndicator *pIndicator;
    bool firstUpdate;

    void setVmId(QString id);
    void setVmName(QString name);
    void startStatusUpdate();


private:
    Ui::VMEle *ui;
    VmStatusUpdateThread *vmStatusUpdateThread;


public slots:
    void statusUpdate(QHash<QString,QString> vmStatusHash);
    void startProcessSlot(HttpRequestWorker *worker);
    void spiceConnectSlot(HttpRequestWorker *worker);
    void spiceConnect2Slot(HttpRequestWorker *worker);
    void rdpConnectSlot(HttpRequestWorker *worker);

private slots:
    void on_startPushButton_clicked();
    void on_shutdownPushButton_clicked();
    void on_restartPushButton_clicked();
    void on_connectsPushButton_clicked();
};

#endif // VMELE_H
