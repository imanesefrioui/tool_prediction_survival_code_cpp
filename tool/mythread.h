#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QProcess>
#include <QTimer>
#include <QProgressBar>


class MyThread: public QThread
{
    Q_OBJECT
signals:
    //void labelsignal(QString);
    //void actualiser(QString);
    void actualiserGUISignal();
    void lireFichierSignal();
    void stoptimerSignal();
    void messageerrorSignal();

private:
    QMutex sync;
    QWaitCondition pauseCond;
    bool pause;
    QProcess *process;

public:
    QTimer *timer;// = new QTimer(this);
    QProgressBar *progressBar;// = new QProgressBar();
    //int sec;

public:
//    MyWorker():pause(false) {}

//    void resume()
//    {
//        sync.lock();
//        pause = false;
//        sync.unlock();
//        pauseCond.wakeAll();
//    }
//    void paused()
//    {
//        sync.lock();
//        pause = true;
//        sync.unlock();
//    }

protected:
    virtual void run();

public slots:
    void printError();
    void printOutput();
    void actualiserGUI();
    void actualiserTimer();
    void stopTimer();
    void closeProgressGui();
    void killProcess();
    //void makepause();

};

#endif // MYTHREAD_H
