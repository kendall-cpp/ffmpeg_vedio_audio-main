#include "Playthread.h"


//音频文件名
#define FILENAME "D:/worksplace/qtworkspace/xiaomage/sdl_player_pcm_home/sdl_player_pcm01/in.pcm"


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

PlayThread::PlayThread(QObject *parent) : QThread(parent)
{
    connect(this, &PlayThread::finished,
            this, &PlayThread::deleteLater);
}

PlayThread::~PlayThread()
{
    disconnect();
    requestInterruption();
    quit();
    wait();
    qDebug() << this << "析构了";
}

int bufferLen;
char *bufferData;

void pull_audio_data(void *userdata,Uint8 *stream,int len) {
    //清空 stream（静音处理）
    SDL_memset(stream,0,len);

    if(bufferLen <=0 ) {
        return ;
    }
    len = (len > bufferLen) ? bufferLen : len;

    //填充数据
    SDL_MixAudio(stream,(Uint8 *) bufferData,len,SDL_MIX_MAXVOLUME);
    bufferData += len;
    bufferLen -= len;
}

void PlayThread::run() {
    //初始化子系统
    if(SDL_Init(SDL_INIT_AUDIO) <0) {
        qDebug() << "SDL_INIT_AUDIO" << SDL_GetError();
        return;
    }
    //音频参数，不同pcm不一样
    SDL_AudioSpec spec;
    //采样率
    spec.freq = SAMPLE_RATE;
    //采样格式（s16le)
    spec.format = AUDIO_S16LSB;
    //声道数
    spec.channels = CHANNELS;
    //音频缓冲区的样本数量（这个值必须是2的倍数）
    spec.samples = 1024;
    //回调函数
    spec.callback = pull_audio_data;

    if(SDL_OpenAudio(&spec,nullptr)) { //返回0 打开成功
        qDebug() << "SDL_OpenAudio error" << SDL_GetError();
          //这时候已经初始化子系统了，所以需要清除
          SDL_Quit();
          return;
    }

    //打开文件
    QFile file(FILENAME);
    if(!file.open((QFile::ReadOnly))) {
        qDebug() << "file open error" << FILENAME;
        //关闭音频设备
        SDL_CloseAudio();
        //消除所有初始化子系统
        SDL_Quit();
        return;
    }
    else {
        qDebug() << "打开文件成功" << FILENAME;
    }
     //开始播放 PCM
    SDL_PauseAudio((0));;  // 0 是取消暂停

    //从存储文件中取数据
    char data[BUFFER_SIZE];
    while(!isInterruptionRequested()) {

        if(bufferLen > 0) continue;

        bufferLen = file.read(data,BUFFER_SIZE);

        //文件数据已经读取完毕
        if(bufferLen <= 0) {
            break;
        }
        bufferData = data;
    }

    file.close();

    SDL_CloseAudio();

    SDL_Quit();

}
