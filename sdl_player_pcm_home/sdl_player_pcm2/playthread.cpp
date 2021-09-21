#include "playthread.h"
#include <qdebug.h>

#include <SDL2/SDL.h>
#include <qdebug.h>
#include <QFile>


//音频文件名
#define FILENAME "D:/worksplace/qtworkspace/xiaomage/record_audio_pro/record_audio_pro/in.pcm"

#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16
//声道数
#define CHANNELS 2
// 音频缓冲区的样本数量
#define SAMPLES 1024
// 每个样本占用多少个字节
#define BYTES_PER_SAMPLE ((SAMPLE_SIZE * CHANNELS) / 8)
// 文件缓冲区的大小
#define BUFFER_SIZE (SAMPLES * BYTES_PER_SAMPLE)

PlayThread::PlayThread(QObject *parent) : QThread(parent) {
    connect(this, &PlayThread::finished,
            this, &PlayThread::deleteLater);

}
//int bufferLen;
//char *bufferData;
//定义成一个结构题
typedef struct {
    int len = 0;
    //每次拉取的数据
    int pulllen = 0;
    Uint8 *data = nullptr;
} AudioBuffer;

PlayThread::~PlayThread() {
    disconnect();
    requestInterruption();
    quit();
    wait();
    qDebug() << this << "析构了";
}



//读取音频回调函数原型
// typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream,int len);
// stream：音频缓冲区（需要将音频数据填充到这个缓冲区）
// len：音频缓冲区的大小（sample * format * channels / 8）
void pull_audio_data(void *userdata,Uint8 *stream,int len) {
    //清空 stream（静音处理）
    SDL_memset(stream,0,len);

    //取出 AudioBuffer,还得把 void* 强壮成 AudioBuffer
    AudioBuffer *buffer = (AudioBuffer*) userdata;

    //文件数据还没准备好
    if(buffer->len <= 0) return;

     // 取len、bufferLen的最小值（为了保证数据安全，防止指针越界）
    buffer->pulllen = (len > buffer->len) ? buffer->len : len;

    //填充数据
    SDL_MixAudio(stream,(Uint8 *) buffer->data,
                 buffer->pulllen,
                 SDL_MIX_MAXVOLUME);
    buffer->data += buffer->pulllen;
    buffer->len -= buffer->pulllen;
}

void PlayThread::run() {

    //showVersion();
     //初始化子系统
 //    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);  //既播放声音播放画面
     if(SDL_Init(SDL_INIT_AUDIO) < 0) {
         qDebug() << "SDL_INIT_AUDIO" << SDL_GetError();
         return;
     }

     //音频参数,不同pcm不一样
     SDL_AudioSpec spec;
     // 采样率
     spec.freq = SAMPLE_RATE;
     // 采样格式（s16le）
     spec.format = AUDIO_S16LSB;
     // 声道数
     spec.channels = CHANNELS;
     // 音频缓冲区的样本数量（这个值必须是2的幂）
     spec.samples = SAMPLES;
     // 回调
     //创建一个结构体
     AudioBuffer buffer;
     spec.userdata = &buffer;  //会把这个地址值传到回调函数
     spec.callback = pull_audio_data;
//     spec.userdata = 100;


     //打开音频设备
 //    extern DECLSPEC int SDLCALL SDL_OpenAudio(SDL_AudioSpec * desired,
 //    SDL_AudioSpec * obtained);
     if(SDL_OpenAudio(&spec,nullptr)) {  //返回 0 就是打开成功
         qDebug() << "SDL_OpenAudio error" << SDL_GetError();
         //这时候已经初始化子系统了，所以需要清除
         SDL_Quit();
         return;
     }

     //打开文件
     QFile file(FILENAME);
     if(!file.open(QFile::ReadOnly)) {
         qDebug() << "file open error" << FILENAME;
         //关闭音频设备
         SDL_CloseAudio();
         //消除所有初始化子系统
         SDL_Quit();
         return;
     }

     //开始播放 PCM
     SDL_PauseAudio(0); //(0 是取消暂停)

     //从存储的文件中读取数据
     Uint8 data[BUFFER_SIZE];
     while(!isInterruptionRequested()) {
         //只要从文件中读取的音频数据，还没有填充完毕，就跳过
         if(buffer.len > 0) continue;

          //rand()函数中接受的是一个char*
         buffer.len = file.read((char *) data,BUFFER_SIZE);

         //文件数据已经读取完毕,推迟线程结束的时间
         if(buffer.len <= 0) {
             //得到的样本数量
             int samples = buffer.pulllen / BYTES_PER_SAMPLE;
             int ms = samples * 1000 / SAMPLE_RATE;
             SDL_Delay(ms);
//             qDebug() << ms;
             break;
         }

         //读取到了文件数据
         buffer.data = data;

         //等待音频数据填充完毕
         //只需要音频数据还没有填充完毕，Delay(sleep)
//         while(bufferLen > 0) {
//             SDL_Delay(1);
//         }
     }

     //关闭文件
     file.close();

     //关闭设备
     SDL_CloseAudio();

     //清除所有子系统
     SDL_Quit();
}
