#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <httprequest.h>
#include <QLabel>
#include "settingdialog.h"
#include "statusupdatethread.h"
#include <qprogressindicator.h>
#include "sessionstatusupdatethread.h"
#include "vmlistwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLabel *currentTimeLabel;
    QLabel *restAPIResultLabel;
    QLabel *currentTimeLabel1;
    SettingDialog *settingDialog;
    StatusUpdateThread *statusUpdateThread;
    QProgressIndicator *pIndicator;
    SessionStatusUpdateThread *seesionStatusUpdate;
    VMListWindow *vmListWindow;

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void closeEvent(QCloseEvent *event);

    void statusUpdate(QString str);

    void on_settingButton_clicked();

    void resetScreentSize();


    void on_loginPushButton_clicked();

    void http_request_result(HttpRequestWorker *worker);

    void sessionStatusUpdateSolts(bool valid);

    void sessionExit(bool sessionExitMode);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
