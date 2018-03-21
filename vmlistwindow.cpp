#include "vmlistwindow.h"
#include "ui_vmlistwindow.h"
#include "vmele.h"
#include "utils.h"
#include "flowlayout.h"

VMListWindow::VMListWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VMListWindow)
{
    ui->setupUi(this);


    //设置主窗口颜色
    this->setObjectName("MainWindow");
    this->setStyleSheet("#MainWindow{border-image:url(:/image/image/backgroud.jpg);}");



    showFullScreen();

    Utils *utils = new Utils();
    connect(utils, &Utils::getVMIDSignal, this, &VMListWindow::updateVmList);
    utils->getVMID();



    //设置流失布局
    flowLayout = new FlowLayout(this);


}

VMListWindow::~VMListWindow()
{
    delete ui;
}

void VMListWindow::on_pushButton_clicked()
{
    emit sessionExit(true);
    this->close();
}

void VMListWindow::updateVmList(QHash<QString, QString> vmHash)
{

    QHash<QString,QString>::iterator it;
    QHash<QString,QString>::iterator currentIt;

    for(it = vmHash.begin(); it != vmHash.end(); ++it){
        if(currentHash.contains(it.key())){

            break;
        }else{
            currentHash.insert(it.key(), it.value());
        }
    }

    for(currentIt = currentHash.begin(); currentIt != currentHash.end(); ++currentIt){
        VMEle *vm = new VMEle();
        vm->setVmId(currentIt.key());
        vm->setVmName(currentIt.value());
        //ui->vmListHorizontalLayout->addWidget(vm);
        flowLayout->addWidget(vm);
        qDebug("updateVmList");
        vm->startStatusUpdate();
    }

    /**
    for(it = vmHash.begin(); it != vmHash.end(); ++it){
        VMEle *vm = new VMEle();
        vm->setVmId(it.key());
        vm->setVmName(it.value());
        //ui->vmListHorizontalLayout->addWidget(vm);
        flowLayout->addWidget(vm);
        qDebug("updateVmList");
        vm->startStatusUpdate();
    }
    */
    //setCentralWidget(ui->vmListWidget);
    ui->vmListWidget->setLayout(flowLayout);
}
