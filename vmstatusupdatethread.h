#ifndef VMSTATUSUPDATETHREAD_H
#define VMSTATUSUPDATETHREAD_H

#include <QThread>
#include <QHash>
#include <QString>
#include "httprequest.h"
#include <QTimer>

class VmStatusUpdateThread : public QThread
{
    Q_OBJECT
public:
    VmStatusUpdateThread(QString id);
    ~VmStatusUpdateThread();
    QString vmID;
    void setVmId(QString id);
    HttpRequestInput *input;
    HttpRequestWorker *worker;



protected:
    void run();
signals:
    void updateStatus(QHash<QString,QString>);

public slots:
    void update(HttpRequestWorker *worker);
    void timeSlot();

};

#endif // VMSTATUSUPDATETHREAD_H
