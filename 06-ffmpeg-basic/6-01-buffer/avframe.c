﻿#include "avframe.h"

#define MEM_ITEM_SIZE (20*1024*102)
#define AVPACKET_LOOP_COUNT 1000
// 测试 内存泄漏
void av_frame_test1()
{
    AVFrame *frame = NULL;
    int ret = 0;

    frame = av_frame_alloc();// 没有类似的AVPacket的av_new_packet的API
    // 1024 *2 * (16/8) =
    frame->nb_samples     = 1024;
    frame->format         = AV_SAMPLE_FMT_S16;//AV_SAMPLE_FMT_S16P AV_SAMPLE_FMT_S16
    frame->channel_layout = AV_CH_LAYOUT_STEREO;    //AV_CH_LAYOUT_MONO AV_CH_LAYOUT_STEREO
    ret = av_frame_get_buffer(frame, 0);    // 根据格式分配内存
    if(frame->buf && frame->buf[0])
        printf("%s(%d) 1 frame->buf[0]->size = %d\n", __FUNCTION__, __LINE__, frame->buf[0]->size);    //受frame->format等参数影响
    if(frame->buf && frame->buf[1])
        printf("%s(%d) 1 frame->buf[1]->size = %d\n", __FUNCTION__, __LINE__, frame->buf[1]->size);    //受frame->format等参数影响

    if(frame->buf && frame->buf[0])        // 打印referenc-counted，必须保证传入的是有效指针
        printf("%s(%d) ref_count1(frame) = %d\n", __FUNCTION__, __LINE__, av_buffer_get_ref_count(frame->buf[0]));

    ret = av_frame_make_writable(frame);    // 当frame本身为空时不能make writable
    printf("av_frame_make_writable ret = %d\n", ret);

    if(frame->buf && frame->buf[0])        // 打印referenc-counted，必须保证传入的是有效指针
        printf("%s(%d) ref_count2(frame) = %d\n", __FUNCTION__, __LINE__, av_buffer_get_ref_count(frame->buf[0]));

    av_frame_unref(frame);
    if(frame->buf && frame->buf[0])        // 打印referenc-counted，必须保证传入的是有效指针
        printf("%s(%d) ref_count3(frame) = %d\n", __FUNCTION__, __LINE__, av_buffer_get_ref_count(frame->buf[0]));

    av_frame_free(&frame);
}


void av_frame_test()
{
    av_frame_test1();
}
