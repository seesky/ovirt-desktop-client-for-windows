#ifndef SESSIONSTATUSUPDATETHREAD_H
#define SESSIONSTATUSUPDATETHREAD_H


#include <QThread>
#include <QTimer>
#include "httprequest.h"

class SessionStatusUpdateThread : public QThread
{
    Q_OBJECT
public:
    SessionStatusUpdateThread();
    ~SessionStatusUpdateThread();

private:
    HttpRequestInput *input;
    HttpRequestWorker *worker;

protected:
    void run();

signals:
    void sessionStatusUpdate(bool);

public slots:
    void timeUpdate();
    void statusUpdate();
};


#endif // SESSIONSTATUSUPDATETHREAD_H
