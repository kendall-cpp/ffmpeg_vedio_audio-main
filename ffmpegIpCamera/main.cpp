#include "mainwindow.h"
#include <iostream>


#include <QApplication>


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


using namespace std;



int main(int argc, char *argv[])
{

    cout << avcodec_version() << endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
