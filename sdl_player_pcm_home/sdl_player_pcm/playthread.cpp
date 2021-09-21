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

PlayThread::~PlayThread() {
    disconnect();
    requestInterruption();
    quit();
    wait();
    qDebug() << this << "析构了";
}

int bufferLen;
char *bufferData;

//读取音频回调函数原型
// typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream,int len);
// stream：音频缓冲区（需要将音频数据填充到这个缓冲区）
// len：音频缓冲区的大小（sample * format * channels / 8）
void pull_audio_data(void *userdata,Uint8 *stream,int len) {
    //清空 stream（静音处理）
    SDL_memset(stream,0,len);

    //文件数据还没准备好
    if(bufferLen <= 0) return;

     // 取len、bufferLen的最小值（为了保证数据安全，防止指针越界）
    len = (len > bufferLen) ? bufferLen : len;

    //填充数据
    SDL_MixAudio(stream,(Uint8 *) bufferData,len,SDL_MIX_MAXVOLUME);
    bufferData += len;
    bufferLen -= len;
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
     spec.samples = 1024;
     // 回调
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
     char data[BUFFER_SIZE];
     while(!isInterruptionRequested()) {
         //只要从文件中读取的音频数据，还没有填充完毕，就跳过
         if(bufferLen > 0) continue;

         bufferLen = file.read(data,BUFFER_SIZE);

         //文件数据已经读取完毕
         if(bufferLen <= 0) break;

         //读取到了文件数据
         bufferData = data;

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
