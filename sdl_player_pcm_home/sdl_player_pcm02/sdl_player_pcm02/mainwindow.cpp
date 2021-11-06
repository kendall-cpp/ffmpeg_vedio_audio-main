#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    if(_playThread) {  //点击之后变成 开始播放
        _playThread->requestInterruption();;
        _playThread = nullptr;
        ui->pushButton->setText("开始播放");
    }
    else {
        _playThread = new PlayThread();
        _playThread->start();
 //       播放结束自动变成开始播放
        connect(_playThread,&PlayThread::finished,[this]{
            _playThread = nullptr;
            ui->pushButton->setText("开始播放");
        });
        ui->pushButton->setText("停止播放");

    }
}
