#include "threadfcrypto.h"
#include "databace.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <sys/time.h>
#include<stdio.h>
unsigned char    key[16] = "heshuanghong";  // to encrypt plain

Threadfcrypto::Threadfcrypto(QObject *parent) :
    QThread(parent)
{
}

void Threadfcrypto::run()
{
    int idnumber= encryption( key );
    unsigned id_max = idnumber -1;
    setflag((int)idnumber,(int)id_max);
    QString fnum = QString::number(idnumber, 10);
    MainWindow::getMainWindowHandle()->SetLineEditText(8,fnum);
    QThread::quit();
}
