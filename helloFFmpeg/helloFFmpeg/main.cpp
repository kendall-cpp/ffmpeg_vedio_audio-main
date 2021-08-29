#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include <cstdio>

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

int main(int argc, char *argv[])
{
    // 打印版本信息
//    qDebug() << av_version_info();
    printf("%s\n",av_version_info());
    avdevice_register_all();


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
