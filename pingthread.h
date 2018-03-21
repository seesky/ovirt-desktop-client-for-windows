#ifndef PINGTHREAD_H
#define PINGTHREAD_H

#include <QThread>

class PingThread : public QThread
{
    Q_OBJECT
public:
    PingThread(QString tagetIPAddress);
    ~PingThread();

protected:
    void run();

signals:
    void pingResult(QString str);
};

#endif // PINGTHREAD_H
