#include "audiothread.h"
#include <qdebug.h>
#include <QFile>

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

#ifdef Q_OS_WIN
    #define FMT_NAME  "dshow"
    #define DEVICE_NAME  "audio=麦克风阵列 (2- Realtek High Definition Audio)"
    // PCM 文件名
    #define FILENAME "D:/lingshengYinShiPin/out.pcm"
#else
    #define FMT_NAME  "avfoundation"
    #define DEVICE_NAME ":0"
    #define FILENAME "~/out.pcm"
#endif


AudioThread::AudioThread(QObject *parent) : QThread(parent)
{
    //点击录音的时候进入子线程
    qDebug() << "录音线程 构造函数";
    //当监听到结束信号时（"finished"),就调用 deleteLater 回收
    connect(this,&AudioThread::finished,
            this,&AudioThread::deleteLater);
}
//析构函数
AudioThread::~AudioThread() {
    //内存回收之前，正常结束线程
    requestInterruption();
    quit();
    wait();
    qDebug() << this << "析构，内存回收";
}

// 当线程启动的时候（start），就会自动调用run函数
// run函数中的代码是在子线程中执行的
// 耗时操作应该放在run函数中
void AudioThread::run() {


    // 获取输入格式对象
    AVInputFormat *fmt =  av_find_input_format(FMT_NAME);
    if(!fmt) {
        qDebug() << "输入格式失败:" << FMT_NAME;
    }

    //格式上下文 （将来可以利用上下文操作设备
    AVFormatContext *ctx = nullptr;
    const char *devicename = DEVICE_NAME;
//    AVDictionary *options = nullptr;
    //打开设备
    int ret = avformat_open_input(&ctx,devicename,fmt,nullptr);
    if(ret < 0) {
        char errorbuf[1024];
        av_strerror(ret,errorbuf,sizeof(errorbuf));
        qDebug() << "打开设备失败";
        return;
    }

       //文件名
    const char *filename = FILENAME;
    QFile file(filename);

     //以只写的方式打开文件，如果文件不存在就创建文件，存在就删除重新创建
    if(!file.open(QIODevice::WriteOnly )) {
        qDebug() << "文件打开失败";
        avformat_close_input(&ctx);
        return;
    }

    //采集传输
//    int count = 50;

    //数据包
    AVPacket pkt;
    //不断采集数据 isInterruptionRequested 是否有人请求我停止
    while( !isInterruptionRequested() && av_read_frame(ctx,&pkt) == 0) {
        file.write((const char*)pkt.data,pkt.size);
    }
//    while( !_stop && av_read_frame(ctx,&pkt) == 0) {
//        file.write((const char*)pkt.data,pkt.size);
//    }

    //关闭文件
    file.close();
    //释放资源
//    av_packet_free(&pkt);
    //关闭设备
    avformat_close_input(&ctx);

    qDebug() << this << "正常结束----------";
}
void AudioThread::setStop(bool stop)
{
    _stop = stop;
}
