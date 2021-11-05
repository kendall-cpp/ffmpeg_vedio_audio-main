#include "mainwindow.h"

#include <QApplication>
#include <qdebug.h>

extern "C"
{
#include <libavformat/avformat.h>
}

int main(int argc, char *argv[])
{
    //打开网路流，这里如果只需要读取本地媒体文件
    avformat_network_init();

    //设置日志级别
    av_log_set_level(AV_LOG_INFO);

    char *default_filename = "believe.mp4";

    char *in_filename = NULL;

    if(argv[1] == nullptr) {
        in_filename = default_filename;

    } else {
        in_filename = argv[1];
    }
//    qDebug() << "file name is:" << in_filename;

    //以日志的形式打印
    av_log(NULL,AV_LOG_INFO,"\nin_filename = %s\n",in_filename);

    AVFormatContext *ifmt_ctx = NULL;  //输入文件的demux（解复用）

    int videoIndex = -1;  //视频索引
    int audioIndex = -1;  //音频索引

    //打开文件，如果是网络文件需要插入网络连接
//int avformat_open_input(AVFormatContext **ps, const char *url, ff_const59 AVInputFormat *fmt, AVDictionary **options);
    int ret = avformat_open_input(&ifmt_ctx,in_filename,NULL,NULL);
    if(ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret,buf,sizeof(buf)-1);
        av_log(NULL,AV_LOG_ERROR,"avformat_open_input %s failed:%s\n",in_filename,buf);;
        return -1;
    }
    //打印输入或者输出格式的信息
    av_log(NULL,AV_LOG_INFO,"\n==== av_dump_format in_filename:%s ===\n",in_filename);
//void av_dump_format(AVFormatContext *ic,int index, const char *url,int is_output);
    av_dump_format(ifmt_ctx,0,in_filename,0);
    av_log(NULL,AV_LOG_INFO,"\n==== av_dump_format finish =======\n\n");

    //查看媒体文件的一些信息
    //打开媒体的路径
    av_log(NULL,AV_LOG_INFO,"media name %s\n",ifmt_ctx->url);
//    媒体流数量
    av_log(NULL,AV_LOG_INFO,"media stream number is %11d\n",ifmt_ctx->nb_streams);
    //bit_rate: 媒体文件的码率,单位为bps
    av_log(NULL,AV_LOG_INFO,"media bit_rate:%11d\n",(ifmt_ctx->bit_rate / 1024));
    //时间
    int total_seconds,hours,minute,second;
    //duration: 媒体文件时长，单位微秒
    total_seconds = (ifmt_ctx->duration) / AV_TIME_BASE;  // 1000us = 1ms, 1000ms = 1秒
    hours = total_seconds / 3600;
    minute = (total_seconds % 3600) / 60;
    second = (total_seconds % 60);
    av_log(NULL,AV_LOG_INFO,"total duration: %02d:%02d:%02d\n", hours, minute, second);



    av_log(NULL,AV_LOG_INFO,"\n");
    return 0;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
}
