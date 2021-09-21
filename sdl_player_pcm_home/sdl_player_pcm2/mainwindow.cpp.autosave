#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qdebug.h>
#include <SDL2/SDL.h>




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

//打印版本号
void showVersion() {
    SDL_version version;
    SDL_VERSION(&version);
    qDebug() << version.major << version.minor << version.patch;
}


//点击事件就触发这个函数
//主线程
void MainWindow::on_playButton_clicked()
{
    if(_playThread) {  //点击之后变成 停止播放
        _playThread->requestInterruption();
        _playThread = nullptr;
        ui->playButton->setText("开始播放");
    }
    else { // 点击之后变成开始播放
           _playThread = new PlayThread();
           _playThread->start();

           //监听线程是否结束，播放结束之后按钮自动编程自动播放
           connect(_playThread,&PlayThread::finished,[this]{
               _playThread = nullptr;
               ui->playButton->setText("开始播放");
           });

           ui->playButton->setText("停止播放");
    }
}
