#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qdebug.h>
#include <SDL2/SDL.h>
#include "playthread.h"



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
    PlayThread *playThread = new PlayThread();
    playThread->start();
}
