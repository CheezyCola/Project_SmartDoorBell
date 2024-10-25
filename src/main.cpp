#include "mainwindow.h"
#include <QApplication>
#include "cstdlib"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    QObject::connect(&w, &MainWindow::exited, &a, &QCoreApplication::quit);
    w.setWindowState(Qt::WindowFullScreen);
    w.show();
    int ret;
    try{
        ret = a.exec();
    }catch (const std::bad_alloc&){
        return EXIT_FAILURE;
    }
    system("xrandr --output DSI-1 --preferred"); // turn on display
    return ret;
}
