#include "mainwindow.h"

#include <QApplication>

#include <qdebug.h>
#include <QString>
#include <QTime>
#include <QFile>
#include <QThread>

extern "C" {
// 设备相关API
#include <libavdevice/avdevice.h>
// 格式相关API
#include <libavformat/avformat.h>
// 工具相关API（比如错误处理）
#include <libavutil/avutil.h>
// 编码相关API
#include <libavcodec/avcodec.h>
}


//设备名称
#ifdef Q_OS_WIN
    #define FMT_NAME  "dshow"
    #define DEVICE_NAME  "audio=麦克风阵列 (2- Realtek High Definition Audio)"
    // PCM 文件名
    #define FILEPATH "D:/YinShiPin/"

#else  //macbook
    #define FMT_NAME  "avfoundation"
    #define DEVICE_NAME ":0"
    #define FILENAME "~/out.pcm"
#endif

int main(int argc, char *argv[])
{

    //注册设备 只候注册一次
    avdevice_register_all();

    qDebug() << "main" << QThread::currentThread();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
