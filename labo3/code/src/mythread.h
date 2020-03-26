#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "threadmanager.h"

#include <QString>

#include <pcosynchro/pcomutex.h>


class MyThread {
private:
    QString passowrdFound;
    bool ended;
    PcoMutex mutexProgress;
    ThreadManager & threadManager;
    QString const charset;
    QString const salt;
    QString const hash;
    long long unsigned nbTotalHash;
public:
    MyThread(
            ThreadManager & threadManager,
            QString const & charset,
            QString const & salt,
            QString const & hash,
            long long unsigned nbTotalHash
            );
    QString getPassword() const;
    void start(
            QVector<unsigned int> currentPasswordArray,
            long long unsigned int nbToCompute
            );
};

#endif // MYTHREAD_H
