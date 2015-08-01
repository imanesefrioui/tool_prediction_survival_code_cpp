#include "mythread.h"
#include "mainwindow.h"
#include <fstream>
#include <unistd.h>
#include <QProcess>
#include <iostream>
#include <QTimer>
#include <QProgressBar>

//MyThread::MyThread()
//{ }
int sec = 0;

void MyThread::run()
{
    progressBar = new QProgressBar();
    //emit labelsignal("running");
    QString string;
    sec=0;
        emit actualiserGUISignal();
        QString program = "java -cp ./weka.jar weka.classifiers.trees.RandomForest -T testFile.csv -l model_rf_656000_linux.model -distribution -p 0";
        system("java -Xmx2500m -cp ./weka.jar weka.classifiers.trees.RandomForest -T testFile.csv -l model_rf_656000_linux.model -distribution -p 0 1>resultat_RF.txt 2>error_RF.txt");
        //process = new QProcess();
        //connect(process,SIGNAL(finished(int)),this,SLOT(printError()));
        //connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
        //connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));
        //process->start(program);  //isRunning = 1;
        //process->waitForFinished(-1);
        //delete process;

        //for avoiding the error QObject::killTimer: timers cannot be stopped from another thread
        //Use signals to start/stop times
             emit stoptimerSignal();
        //and for avoiding java heap size error
             qDebug() << "WARNING: maybe java heap size ! WARNING: or maybe Destroyed while process (java) is still running";
             emit messageerrorSignal();
        //read the result file in case no error
             emit lireFichierSignal();
}

void MyThread::printError()
{
    std::ofstream outStream;
    outStream.open("error_RF.txt",std::ios_base::out | std::ios_base::app);
    outStream<<QVariant(process->readAllStandardError()).toString().toStdString();
    outStream.close();
}
void MyThread::printOutput()
{
    std::ofstream outStream;
    outStream.open("resultat_RF.txt",std::ios_base::out | std::ios_base::app);
    outStream<<QVariant(process->readAllStandardOutput()).toString().toStdString();
    outStream.close();
}
void MyThread::actualiserGUI()
{
    timer->start(); //il declenche connect(timer, SIGNAL(timeout())...
    timer->setInterval(4000);
    sec = 0;
    progressBar->setWindowTitle("Remaining Time");
    progressBar->setMinimum(1);
    progressBar->setMaximum(100);
    progressBar->move(400, 540);
    progressBar->setFixedSize(280, 15);
    progressBar->setValue(0);
    progressBar->setVisible(true);
    //connect(timer, SIGNAL(timeout()), this, SLOT(actualiserTimer()));
}
void MyThread::actualiserTimer()
{
    sec++;
    //QString string= QString("Valeur %1").arg(sec);
    progressBar->setValue(sec+1);
}
void MyThread::closeProgressGui()
{
    progressBar->close();
    delete progressBar;
    progressBar = new QProgressBar();
}
void MyThread::killProcess()
{
    process->kill();
    progressBar->close();
}
void MyThread::stopTimer()
{
    timer->stop();
    delete timer;
    timer = new QTimer(this);
}
//void MyThread::makepause()
//{
//    sync.lock();
//    pause = true;
//    //w.wait();
//    emit labelsignal("string");
//    sync.unlock();
//}
