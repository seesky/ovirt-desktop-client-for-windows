#ifndef EXECCMD_H
#define EXECCMD_H

#include <QThread>

class ExecCMD : public QThread
{
    Q_OBJECT
public:
    ExecCMD(QString cmd, long s = 0);
    ~ExecCMD();

protected:
    void run();
signals:
    void exec(QString result);
};

#endif // EXECCMD_H
