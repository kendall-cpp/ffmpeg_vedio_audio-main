#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    qDebug() << "on_pushButton_clicked" << QThread::currentThread();
    return;

     // 获取输入格式对象
//ff_const59 AVInputFormat *av_find_input_format(const char *short_name);
    AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if(!fmt) {
        qDebug() << "输入格式失败:" << FMT_NAME;
        return;
    }
    else {
        qDebug() << "输入格式成功:" << FMT_NAME;
    }

    //格式上下文 （将来可以利用上下文操作设备
    AVFormatContext *ctx = nullptr;
    //设备名称
    const char *device_name = DEVICE_NAME;
//int avformat_open_input(AVFormatContext **ps, const char *url, ff_const59 AVInputFormat *fmt, AVDictionary **options);
     int ret = avformat_open_input(&ctx,device_name,fmt,nullptr);
     if(ret < 0) {
         char errorbuf[1024];
         av_strerror(ret,errorbuf,sizeof(errorbuf));
         qDebug() << "打开设备失败";
         return;
     }


     //采集数据，一帧一帧采集


     //文件名
     QString filename = FILEPATH;
     filename += "out.pcm";
     QFile file(filename);  //构建文件

     //以只写的形式打开文件，如果文件不存在就创建文件，存在就删除重新创建
     if(!file.open((QIODevice::WriteOnly))) {
         qDebug() << "文件打开失败";
         avformat_close_input(&ctx);
         return;
     }

     //数据包
//     AVPacket pkt;
//     av_read_frame(ctx,&pkt);
//     qDebug() << pkt.size ;

//     采集传输50次
     int count = 5;

    AVPacket *pkt = av_packet_alloc();
    //不断采集数据
    while(count-- > 0 && av_read_frame(ctx,pkt) == 0) {
        qDebug() << pkt->size;
        file.write((const char*) pkt->data,pkt->size);
    }

    //释放资源
    file.close();

    //关闭设备
    avformat_close_input(&ctx);

}




