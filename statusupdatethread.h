#ifndef STATUSUPDATETHREAD_H
#define STATUSUPDATETHREAD_H

#include <QThread>
#include <QTimer>

class StatusUpdateThread : public QThread
{
    Q_OBJECT
public:
    StatusUpdateThread();
    ~StatusUpdateThread();

private:


protected:
    void run();

signals:
    void updateStatus(QString str);

public slots:
    void update();




};

#endif // STATUSUPDATETHREAD_H
