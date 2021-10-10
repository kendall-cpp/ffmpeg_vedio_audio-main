#include <stdio.h>
#include <libavformat/avformat.h>


void av_demux_text(char *in_filename) {

    //打开网络流。这里如果只需要读取本地媒体文件，不需要用到网络功能，可以不用加上这一句
//    avformat_network_init();

    //如果不输入参数的话就
    char *default_name = "believe.mp4";
    if(in_filename == NULL)  {
        in_filename = default_name;
    }
    printf("in_filename = %s\n",in_filename);

    //AVFormatContext是描述一个媒体文件或媒体流的构成和基本信息的结构体
    AVFormatContext *ifmt_ctx = NULL;

    int videoindex = -1;  //视频索引
    int audioindex = -1;  //音频索引

    //打开本地媒体文件  avformat_open_input: 打开多媒体数据并且获取一些信息
    int ret = avformat_open_input(&ifmt_ctx,in_filename,NULL,NULL);
    if(ret < 0) {
        char buf[1024] = {0};
//        三个参数分别是错误码\用户设置的错误描述缓存\缓存大小.
        av_strerror(ret,buf,sizeof(buf) - 1); //获取错误信息
        printf("open %s is failed: %s\n",in_filename,buf);
        goto failed;  //需要关闭上下文
    }
    //读取pkt并提取媒体流中的信息
    ret = avformat_find_stream_info(ifmt_ctx,NULL);
    if(ret < 0) {
        char buf[1024] = {0};
        av_strerror(ret,buf,sizeof(buf) - 0);
        printf(("avformat_find_stream_info %s failed: %s",in_filename,buf));
        goto failed;
    }

    //走到这里说明打开媒体文件和从媒体流中提取信息成功
//    打印关于输入或输出格式的详细信息
    printf_s("\n==== av_dump_format in_filename:%s ===\n", in_filename);
 /*
  * printf只是检查format字符串是否为空。如果为空，会触发assert(false)。
    而printf_s还会检查format字符串是否有效，即其中格式是否合法。如果不合法，也会触发assert(false)。
  * */
//    void av_dump_format(AVFormatContext *ic, int index, const char *url,int is_output);
    av_dump_format(ifmt_ctx,0,in_filename,0);  //打印出媒体流的上下文信息
    printf_s("\n==== av_dump_format finish =======\n\n");

    printf("media name: %s\n",ifmt_ctx->url);           //打印媒体 路径/名字

    printf("stream number: %d\n", ifmt_ctx->nb_streams);  // nb_streams: nb_streams媒体流数量

    printf("media average ratio: %lldkbps\n",(int64_t)(ifmt_ctx->bit_rate/1024));

    //媒体文件时长，单位微妙
    int total_seconds,hour,minute,second;
    total_seconds = (ifmt_ctx->duration) / AV_TIME_BASE;  //#define AV_TIME_BASE 1000 000 1000us = 1ms, 1000ms = 1秒
    hour = total_seconds / 3600;
    minute = (total_seconds % 3600) / 60;
    second = (total_seconds % 60);
    //可以得到媒体文件的总时长
    printf("media time: %02d:%02d:%02d\n",hour,minute,second);
    printf("\n");

//    //////////////////////////////
//    printf("------- get medio info ---------------\n");
////    新版本的 FFmepg 中增加了函数 av_find_best_stream(),也可以取得同样的效果
////    int  audioStream = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
////    printf("audioStream:%d\n",audioStream);

////    av_find_best_stream函数其实跟以前实现思路是一样的，只不过这儿的条件更多一点而己。
//    // 为了方便理解内部实现逻辑，使用老版本通过遍历的方式读取媒体文件视频和音频信息
//    for(int i=0;i< ifmt_ctx->nb_streams;++i) {
//        AVStream *in_stream = ifmt_ctx->streams[i]; //音频流，视频流，字幕流 [注意不是按顺序]
//        //音频流
//        if(AVMEDIA_TYPE_AUDIO == in_stream->codecpar->codec_type) {
//            printf("----- Aduio info------------- \n");
//            // sample_rate: 音频编解码器的采样率，单位为Hz
//            printf("samplerate:%dHz\n",in_stream->codecpar->sample_rate);
//             // codecpar->format: 音频采样格式
//            if(AV_SAMPLE_FMT_FLTP == in_stream->codecpar->format) {
//                printf("sampleformat:AV_SAMPLE_FMT_FLTP\n");
//            }
//            else if(AV_SAMPLE_FMT_S16P == in_stream->codecpar->format) {
//                printf("sampleformat: AV_SAMPLE_FMT_S16P\n");
//            }
//            //channels:音频信道数目
//            printf("channel number: %d\n",in_stream->codecpar->channels);
//            // codec_id: 音频压缩编码格式
//            if(AV_CODEC_ID_AAC == in_stream->codecpar->codec_id) {
//                printf("audio codec: AAC\n");
//            }
//            else if (AV_CODEC_ID_MP3 == in_stream->codecpar->codec_id) {
//                printf("audio codec: MP3\n");
//            }
//            else {
//                printf("audio codec_id:%d\n", in_stream->codecpar->codec_id);
//            }
//            //音频总时长
//            //如果把单位放大为毫秒或者微妙，音频总时长跟视频总时长不一定相等的
//            if(in_stream->duration != AV_NOPTS_VALUE) { //AV_NOPTS_VALUE 未定义的时间戳
//                int duration_audio = (in_stream->duration) * av_q2d(in_stream->time_base); //av_q2d 将AVRational转换为' double '
//                printf("audio duration: %02d:%02d:%02d\n",
//                       duration_audio / 3600, (duration_audio % 3600) / 60, (duration_audio % 60));
//            }
//            else {
//                printf("audio duration unknown");;
//            }
//            printf(("\n"));

//            //标记音频索引
//            audioindex = i;
//            printf("audioindex: %d\n",audioindex);  //1
//        }
//        else if(AVMEDIA_TYPE_VIDEO == in_stream->codecpar->codec_type) {
//            printf("------- Video info--------------------- \n");
//            printf("video index: %d\n");
//            // avg_frame_rate: 视频帧率,单位为fps，表示每秒出现多少帧
//            printf("fps:%1ffps\n",av_q2d(in_stream->avg_frame_rate));
//            if(AV_CODEC_ID_MPEG4 == in_stream->codecpar->codec_id) { //视频压缩编码格式
//                printf("video codec:MPEG4\n");
//            }
//            else if(AV_CODEC_ID_H264 == in_stream->codecpar->codec_id) {
//                printf("video codec:H264\n");
//            }
//            else {
//                printf("video codec_id:%d\n", in_stream->codecpar->codec_id);
//            }
//            //视频帧宽度和高度
//            printf("width:%d height:%d\n", in_stream->codecpar->width,
//                   in_stream->codecpar->height);
//            //视频总时长，单位为秒。
//            //注意如果把单位放大为毫秒或者微妙，音频总时长跟视频总时长不一定相等的
//            if(in_stream->duration != AV_NOPTS_VALUE)
//            {
//                int duration_video = (in_stream->duration) * av_q2d(in_stream->time_base);
//                printf("video duration: %02d:%02d:%02d\n",
//                       duration_video / 3600,
//                       (duration_video % 3600) / 60,
//                       (duration_video % 60)); //将视频总时长转换为时分秒的格式打印到控制台上
//            }
//            else
//            {
//                printf("video duration unknown");
//            }
//            printf("\n");
//            videoindex = i;
//            printf("videoindex: %d\n",videoindex);
//        }
//    }


//    //获取视频的数据  获取视频的一帧，不存在半帧说法。但可以获取音频的若干帧
//    AVPacket *pkt = NULL;

//    int pkt_count = 0;
//    int print_max_count = 10;
//    pkt = av_packet_alloc();
//    printf("\n-----av_read_frame start\n");
//    while(1) {
//        ret = av_read_frame(ifmt_ctx,pkt);
//        if (ret < 0)
//        {
//            printf("av_read_frame end\n");
//            break;
//        }
//        if(pkt_count++ < print_max_count) {
//            //如果这一帧是音频帧
//            if(pkt->stream_index == audioindex) {
//                printf("audio pts: %lld\n", pkt->pts);
//                  printf("audio dts: %lld\n", pkt->dts);
//                  printf("audio size: %d\n", pkt->size);
//                  printf("audio pos: %lld\n", pkt->pos);
//                  printf("audio duration: %lf\n\n",
//                         pkt->duration * av_q2d(ifmt_ctx->streams[audioindex]->time_base));
//            }
//            //如果这一帧是视频帧
//            else if(pkt->stream_index == videoindex) {
//                printf("video pts: %lld\n", pkt->pts);
//                printf("video dts: %lld\n", pkt->dts);
//                printf("video size: %d\n", pkt->size);
//                printf("video pos: %lld\n", pkt->pos);
//                printf("video duration: %lf\n\n",
//                       pkt->duration * av_q2d(ifmt_ctx->streams[videoindex]->time_base));
//            }
//            else {
//                printf("unknown stream_index:\n", pkt->stream_index);
//            }
//        }
//    }
//    if(pkt) {
//        av_packet_free(&pkt);
//    }

 failed:
    //如果不为空就要关闭上下文
    if(ifmt_ctx) {
        avformat_close_input(&ifmt_ctx);
    }
    getchar();
    return;
}
