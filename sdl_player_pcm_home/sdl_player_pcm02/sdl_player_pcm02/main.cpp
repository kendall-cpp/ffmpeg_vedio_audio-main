#include "mainwindow.h"

#include <QApplication>


//查明原因是SDL和QT同时定义了main。
#undef main
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
