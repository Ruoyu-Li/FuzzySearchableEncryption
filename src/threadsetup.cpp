#include "threadsetup.h"
#include "mainwindow.h"

threadsetup::threadsetup(QObject *parent) :
    QThread(parent)
{
}
void threadsetup::run()
{
    MainWindow::getMainWindowHandle()->setup();
    QThread::quit();
}
