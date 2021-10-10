#include "avpacket.h"


#define MEM_ITEM_SIZE (20*1024*102)
#define AVPACKET_LOOP_COUNT 1000
// 测试 内存泄漏


/**
 * 测试av_packet_alloc和av_packet_free的配对使用
 */
void av_packet_test1()
{
    AVPacket *pkt = NULL;
    int ret = 0;

    pkt = av_packet_alloc();
    ret = av_new_packet(pkt, MEM_ITEM_SIZE);  //这里永远都是返回0，返回的并不是引用计数
//    printf("ret = %d\n",ret);  //0
    //把数据写到data区域,  av_init_packet 内部初始化并不是涉及 data 和 size
    memccpy(pkt->data,(void*)&av_packet_test1,1,MEM_ITEM_SIZE);
    av_packet_free(&pkt);

}






/**
 * @brief 测试误用av_init_packet将会导致内存泄漏
 */
void av_apacket_test2() {
    AVPacket *pkt = NULL;
    int ret = 0;
    pkt = av_packet_alloc();
    ret = av_new_packet(pkt,MEM_ITEM_SIZE);
    memccpy(pkt->data,(void*)&av_apacket_test2,1,MEM_ITEM_SIZE);
//    av_init_packet(pkt);
    //如果在av_packet_free前调用 av_init_packet 就会把 buf 置空，这样的话在 av_packet_free 内部就会直接 return
    //导致 buf 无法释放，所以会导致内存泄漏

    av_packet_free(&pkt);
}

/**
 * @brief 测试av_packet_move_ref后，可以av_init_packet
 */
void av_packet_test3() {
    AVPacket *pkt1 = NULL;
    AVPacket *pkt2 = NULL;
    int ret = 0;

    pkt1 = av_packet_alloc();
    ret = av_new_packet(pkt1,MEM_ITEM_SIZE);
    memccpy(pkt1->data,(void*)&av_packet_test3,1,MEM_ITEM_SIZE);
    pkt2 = av_packet_alloc();  //如果没有分配空间调试会报错

    av_frame_move_ref(pkt2,pkt1);  //这时候pkt1 的 buf 已经置空了，内部调用了av_init_packet

    av_packet_free(&pkt1);
    av_packet_free(&pkt2);
}
//测试av_packet_clone
void av_packet_test4() {
    AVPacket *pkt1 = NULL;
    AVPacket *pkt2 = NULL;
    int ret = 0;

    pkt1 = av_packet_alloc();
    pkt2 = av_packet_alloc();

    ret= av_new_packet(pkt1,MEM_ITEM_SIZE);
    memccpy(pkt1->data, (void *)&av_packet_test1, 1, MEM_ITEM_SIZE);
    pkt2 = av_packet_clone(pkt1);  // 和av_frame_move_ref的区别是pkt1的buf不会置空

    av_packet_free(&pkt1);
    av_packet_free(&pkt2);
}

void av_packet_test6() {
    AVPacket *pkt1 = NULL;
    AVPacket *pkt2 = NULL;
    int ret = 0;

    pkt1 = av_packet_alloc();
    pkt2 = av_packet_alloc();
    ret = av_new_packet(pkt1,MEM_ITEM_SIZE);
    memccpy(pkt1->data,(void*)&av_packet_test6,1,MEM_ITEM_SIZE);
    *pkt2 = *pkt1;

    av_packet_free(&pkt1);
    av_packet_free(&pkt2);
}

void av_packet_test7() {
    AVPacket *pkt = NULL;
    av_init_packet(pkt);
}


void av_packet_test()
{
    av_packet_test1();
}
