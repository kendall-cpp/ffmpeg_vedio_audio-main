#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "Mainwindow" << QThread::currentThread();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(_audioThread == nullptr) {   // 如果点击了开始录音
        _audioThread = new audioThread(this);
        _audioThread->start();
        ui->pushButton->setText("结束录音");
    } else {
        qDebug() << "---------------------------";
        //结束线程
        _audioThread->setStop(true);
        _audioThread = nullptr;

        ui->pushButton->setText("开始录音");
    }


    return;

}




