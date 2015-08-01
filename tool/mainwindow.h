#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "mythread.h"
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void predict();
    void generateNewPatient();
    void printError();
    void printOutput();
    void progress();
    void lireFichier();
    void actualiserProgressUi();
    void messageError();
    void stopTimerUi();

private slots:
    void deleteBayesianModel();
    void deleteJ48Model();
    void deleteAnnModel();
    void deleteRandomForestModel();
    void deleteSvmModel();
    void actualiserTimer();
    void deleteMessageBox();

private:
    Ui::MainWindow *ui;
    QProcess *process;


    QTimer *timerUi;// = new QTimer(this);
    int secondUi;
    QMessageBox *m_error;// = new QMessageBox(this);
    MyThread worker;

    void generateTestFile();
    void predictNaiveBayes();
    void createBayesianModelFile();

    void predictJ48();
    void createJ48ModelFile();

    void predictAnn();
    void createAnnModelFile();

    void predictRandomforest();
    void createRandomforestModelFile();

    void predictSvm();
    void createSvmModelFile();

signals:
    void closeProgressGuiandDeleteMessageBoxSignal();

};


#endif // MAINWINDOW_H
