#ifndef VMLISTWINDOW_H
#define VMLISTWINDOW_H

#include <QMainWindow>
#include <QHash>
#include "flowlayout.h"

namespace Ui {
class VMListWindow;
}

class VMListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VMListWindow(QWidget *parent = 0);
    ~VMListWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::VMListWindow *ui;
    QHash<QString, QString> vmsHash;
    QHash<QString, QString> currentHash;
    FlowLayout *flowLayout;

public slots:
    void updateVmList(QHash<QString, QString>);


signals:
    void sessionExit(bool);
};

#endif // VMLISTWINDOW_H
