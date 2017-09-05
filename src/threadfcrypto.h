#ifndef THREADFCRYPTO_H
#define THREADFCRYPTO_H

#include <QThread>

class Threadfcrypto : public QThread
{
    Q_OBJECT
public:
    explicit Threadfcrypto(QObject *parent = 0);
    void run();
signals:

public slots:

};

#endif // THREADFCRYPTO_H
