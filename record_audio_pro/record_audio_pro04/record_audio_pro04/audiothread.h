#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <qdebug.h>
#include <qdebug.h>
#include <QString>
#include <QTime>
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

class audioThread : public QThread
{
    Q_OBJECT
public:
    explicit audioThread(QObject *parent = nullptr);
    void setStop(bool stop);

    ~audioThread();

private:
    void run();

    bool _stop = false;


signals:

};

#endif // AUDIOTHREAD_H
