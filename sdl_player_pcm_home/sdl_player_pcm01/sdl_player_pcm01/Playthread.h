#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QObject>
#include <QThread>
#include <qdebug.h>
#include <QFile>

#include <SDL2/SDL.h>


class PlayThread : public QThread
{
    Q_OBJECT
public:
    explicit PlayThread(QObject *parent = nullptr);
    ~PlayThread();

private:
    void run();
signals:

};

#endif // PLAYTHREAD_H
