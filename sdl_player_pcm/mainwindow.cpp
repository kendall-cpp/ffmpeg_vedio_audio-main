#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SDL2/SDL.h>
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
}

//打印版本号
void showVersion() {
    SDL_version version;
    SDL_VERSION(&version);
    qDebug() << version.major << version.minor << version.patch;
}

void MainWindow::on_playButton_clicked()
{
//    showVersion();
    //初始化子系统
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        qDebug() << "SDL_INIT_AUDIO" << SDL_GetError();
        return;
    }

    //清除所有子系统
    SDL_Quit();
}
