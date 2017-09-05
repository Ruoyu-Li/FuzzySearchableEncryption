#ifndef THREADSETUP_H
#define THREADSETUP_H

#include <QThread>

class threadsetup : public QThread
{
    Q_OBJECT
public:
    explicit threadsetup(QObject *parent = 0);
    void run();
signals:

public slots:

};

#endif // THREADSETUP_H
