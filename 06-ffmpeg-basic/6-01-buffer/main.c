#include <stdio.h>
#include "avpacket.h"
#include "avframe.h"

#include "demux.h"
int main(int argc, char **argv)
{
//    printf("Hello World!\n");
//    av_packet_test();
//    av_frame_test();


    // 测试解封装
    char *in_filename = argv[1];
    av_demux_text(in_filename);
    return 0;
}
