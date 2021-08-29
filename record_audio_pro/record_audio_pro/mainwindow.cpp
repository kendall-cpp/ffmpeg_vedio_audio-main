#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio>

#include <qdebug.h>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "窗口线程析构";
}


void MainWindow::on_audioButton_clicked()
{
    if(_audioThread == nullptr) {
        //开启录音线程
        _audioThread = new AudioThread(this);  //这里必须传递 this，才会关闭窗口线程
        _audioThread->start();
        ;
        //设置按钮文字
        ui->audioButton->setText("结束录音");
    }else {
        //结束录音线程
//        _audioThread->setStop(true);
        //请求结束
        _audioThread->requestInterruption();

        //让线程销毁，而不是仅仅改变状态
        _audioThread = nullptr;

        //设置按钮文字
        ui->audioButton->setText("开始录音");
    }
}
