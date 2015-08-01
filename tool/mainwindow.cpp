#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QProcess>
#include <QMessageBox>
#include <unistd.h>
#include <QFile>
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->predictPushButton,SIGNAL(clicked()),this,SLOT(predict()));
    QObject::connect(ui->newPatientPushButton,SIGNAL(clicked()),this,SLOT(generateNewPatient()));
    setWindowTitle("Calculator: Prediction of Survival");
    ui->probaLabel->setText("Probability: ");
    ui->classLabel->setText("Class: ");
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::predict()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    //ui->progressBar->setMinimum(0); ui->progressBar->setMaximum(0);// for a busy progress bar or 'GUI element'
    ui->progressBar->setValue(0);
    ui->probaLabel->setText("Probability: ");
    ui->classLabel->setText("Class: ");
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");

    this->generateTestFile();
    if(ui->naiveBayesRadioButton->isChecked())
        this->predictNaiveBayes();
    else if(ui->id3RadioButton->isChecked())
        this->predictJ48();
    else if(ui->annRadioButton->isChecked())
        this->predictAnn();
    else if(ui->svmRadioButton->isChecked())
        this->predictSvm();
    else if(ui->randomForestRadioButton->isChecked())
    {
        this->predictRandomforest();
    }
}


void MainWindow::generateTestFile()
{
    std::ofstream outFile;
    outFile.open("testFile.csv");
    outFile<<"AGE,GENDER,INJURYTYPE,EDSYSBP,EDRESPRATE,GCSEYE,GCSVERBAL,GCSMOTOR,Abdomen,Face,Head,Neck,Thorax,UpperExtremity,LowerExtremity,External,Spine,DISSTATUS"<<std::endl;
    outFile<<"34,'Female','Blunt      ',96,24,3,5,6,0,1,1,0,0,2,0,0,0,'Alive'"<<std::endl;
    outFile<<"39,'Male','Penetrating',151,22,4,5,6,0,0,0,0,0,0,0,0,1,'Dead '"<<std::endl;
    outFile<<ui->ageSpinBox->value()<<",'"<<ui->genderComboBox->currentText().toStdString()<<"','";
    outFile<<ui->typeOfInjuryComboBox->currentText().toStdString()<<"',"<<ui->bloodPressureSpinBox->value();
    outFile<<","<<ui->respirationRateSpinBox->value()<<","<<(ui->eyeMouvementComboBox->currentIndex()+1);
    outFile<<","<<(ui->verbalResponsecomboBox->currentIndex()+1)<<",";
    outFile<<(ui->motorCoordinationComboBox->currentIndex()+1)<<",";
    outFile<<(ui->abdomenComboBox->currentIndex())<<",";
    outFile<<(ui->faceComboBox->currentIndex())<<",";
    outFile<<(ui->headComboBox->currentIndex())<<",";
    outFile<<(ui->neckComboBox->currentIndex())<<",";
    outFile<<(ui->thoraxComboBox->currentIndex())<<",";
    outFile<<(ui->upperExtremityComboBox->currentIndex())<<",";
    outFile<<(ui->lowerExtremityComboBox->currentIndex())<<",";
    outFile<<(ui->externalComboBox->currentIndex())<<",";
    outFile<<(ui->spineComboBox->currentIndex())<<",";
    outFile<<"?";
    outFile.close();
}


void MainWindow::printError()
{
    std::ofstream outStream; emit ui->progressBar->setValue(60);
    outStream.open("error.txt",std::ios_base::out | std::ios_base::app);
    outStream<<QVariant(process->readAllStandardError()).toString().toStdString();
    outStream.close();
}
void MainWindow::printOutput()
{
    std::ofstream outStream; emit ui->progressBar->setValue(100*0.70);
    outStream.open("resultat.txt",std::ios_base::out | std::ios_base::app);
    outStream<<QVariant(process->readAllStandardOutput()).toString().toStdString();
    outStream.close();
}
void MainWindow::progress()
{
int val=1;
    while( (val<ui->progressBar->maximum()) & (val<50) )
    {ui->progressBar->setValue(val++);
    usleep(1);}
}


























void MainWindow::predictNaiveBayes()
{
    std::ofstream outStream;
    outStream.open("resultat.txt");
    outStream.close();
    outStream.open("error.txt");
    outStream.close();
    this->createBayesianModelFile();
    process= new QProcess();
    QString prog = "java -cp \"./weka.jar\" weka.classifiers.bayes.NaiveBayes -T testFile.csv -l model_naive_656000_linux.model -distribution -p 0";
    QObject::connect(process,SIGNAL(started()),this,SLOT(progress()));
    QObject::connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
    QObject::connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));

    //QObject::connect(process,SIGNAL(finished(int)),this,SLOT(deleteBayisianModel()));
    process->start(prog);
    process->waitForFinished(-1);
    delete process;
    this->deleteBayesianModel();

    std::ifstream in;
    in.open("resultat.txt");
    std::string ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;
    in>>ch;
    QString classString(ch.c_str());
    classString=classString.split(":").at(1);
    in>>ch;
    QString probaString(ch.c_str());
    probaString=probaString.split(",").at(0);
    if(probaString.size()>1)
    {
        probaString.replace("*"," ");

    }

    ui->probaLabel->setText("Probability: "+probaString);
    ui->classLabel->setText("Class: "+classString);
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");

    emit ui->progressBar->setValue(100*1);
    ui->progressBar->show();
}
void MainWindow::createBayesianModelFile()
{
    //QFile::copy(":/files/naive_100000_model_win.model","./naive_100000_model_win.model");
    QFile::copy(":/files/model_naive_656000_linux.model","./model_naive_656000_linux.model");
    // the file will be compiled with the executable program .exe so there is no need to provide the file
    // when you create a resource, it is compiled and included in the executable
}
void MainWindow::deleteBayesianModel()
{
    //QFile file("./naive_100000_model_win.model");
    QFile file("./model_naive_656000_linux.model");
    file.setPermissions(QFile::WriteOther);
    file.remove();
}














void MainWindow::predictJ48()
{
    std::ofstream outStream;
    outStream.open("resultat.txt");
    outStream.close();
    outStream.open("error.txt");
    outStream.close();
    this->createJ48ModelFile();
    process= new QProcess();
    QString prog = "java -cp \"./weka.jar\" weka.classifiers.trees.J48 -T testFile.csv -l model_j48_656000_linux.model -distribution -p 0";
    QObject::connect(process,SIGNAL(started()),this,SLOT(progress()));
    QObject::connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
    QObject::connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));
    process->start(prog);
    process->waitForFinished(-1);
    delete process;
    this->deleteJ48Model();

    std::ifstream in;
    in.open("resultat.txt");
    std::string ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;
    in>>ch;
    QString classString(ch.c_str());
    classString=classString.split(":").at(1);
    in>>ch;
    QString probaString(ch.c_str());
    probaString=probaString.split(",").at(0);
    if(probaString.size()>1)
    {
        probaString.replace("*"," ");

    }
    ui->probaLabel->setText("Probability: "+probaString);
    ui->classLabel->setText("Class: "+classString);
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    emit ui->progressBar->setValue(100*1);
    ui->progressBar->show();
}
void MainWindow::createJ48ModelFile()
{
    QFile::copy(":/files/model_j48_656000_linux.model","./model_j48_656000_linux.model");
}
void MainWindow::deleteJ48Model()
{
    QFile file("./model_j48_656000_linux.model");
    file.setPermissions(QFile::WriteOther);
    file.remove();
}















void MainWindow::predictAnn()
{
    std::ofstream outStream;
    outStream.open("resultat.txt");
    outStream.close();
    outStream.open("error.txt");
    outStream.close();
    this->createAnnModelFile();
    process= new QProcess();
    QString prog = "java -cp \"./weka.jar\" weka.classifiers.functions.MultilayerPerceptron -T testFile.csv -l model_ann_656000_linux.model -distribution -p 0";
    QObject::connect(process,SIGNAL(started()),this,SLOT(progress()));
    QObject::connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
    QObject::connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));
    process->start(prog);
    process->waitForFinished(-1);
    delete process;
    this->deleteAnnModel();

    std::ifstream in;
    in.open("resultat.txt");
    std::string ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;
    in>>ch;
    QString classString(ch.c_str());
    classString=classString.split(":").at(1);
    in>>ch;
    QString probaString(ch.c_str());
    probaString=probaString.split(",").at(0);
    if(probaString.size()>1)
    {
        probaString.replace("*"," ");

    }
    ui->probaLabel->setText("Probability: "+probaString);
    ui->classLabel->setText("Class: "+classString);
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    emit ui->progressBar->setValue(100*1);
    ui->progressBar->show();
}
void MainWindow::createAnnModelFile()
{
    QFile::copy(":/files/model_ann_656000_linux.model","./model_ann_656000_linux.model");
}
void MainWindow::deleteAnnModel()
{
    QFile file("./model_ann_656000_linux.model");
    file.setPermissions(QFile::WriteOther);
    file.remove();
}



























void MainWindow::predictSvm()
{
    std::ofstream outStream;
    outStream.open("resultat.txt");
    outStream.close();
    outStream.open("error.txt");
    outStream.close();
    this->createSvmModelFile();
    process= new QProcess();
    QString prog = "java -cp \"./weka.jar\" weka.classifiers.functions.SMO -T testFile.csv -l model_smo_656000_linux.model -distribution -p 0";
    QObject::connect(process,SIGNAL(started()),this,SLOT(progress()));
    QObject::connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
    QObject::connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));
    process->start(prog);
    process->waitForFinished(-1);
    delete process;
    this->deleteSvmModel();

    std::ifstream in;
    in.open("resultat.txt");
    std::string ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
    in>>ch;in>>ch;in>>ch;
    in>>ch;
    QString classString(ch.c_str());
    classString=classString.split(":").at(1);
    in>>ch;
    QString probaString(ch.c_str());
    probaString=probaString.split(",").at(0);
    if(probaString.size()>1)
    {
        probaString.replace("*"," ");

    }
    ui->probaLabel->setText("Probability: "+probaString);
    ui->classLabel->setText("Class: "+classString);
    ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");
    emit ui->progressBar->setValue(100*1);
    ui->progressBar->show();
}
void MainWindow::createSvmModelFile()
{
    QFile::copy(":/files/model_smo_656000_linux.model","./model_smo_656000_linux.model");
}
void MainWindow::deleteSvmModel()
{
    QFile file("./model_smo_656000_linux.model");
    file.setPermissions(QFile::WriteOther);
    file.remove();
}














void MainWindow::predictRandomforest()
{
        ui->newPatientPushButton->setEnabled(false);
        ui->annRadioButton->setEnabled(false);
        ui->svmRadioButton->setEnabled(false);
        ui->naiveBayesRadioButton->setEnabled(false);
        ui->id3RadioButton->setEnabled(false);
        timerUi = new QTimer(this);
        m_error = new QMessageBox(this);
        std::ofstream outStream;
        outStream.open("resultat_RF.txt");
        outStream.close();
        outStream.open("error_RF.txt");
        outStream.close();
        ui->label_17->setText("Estimated time 1min");
        //this->createRandomforestModelFile();
        //process= new QProcess();
        //QString prog = "java -cp \"./weka.jar\" weka.classifiers.trees.RandomForest -T testFile.csv -l model_rf_656000_linux.model -distribution -p 0";
        //QObject::connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
        //QObject::connect(process,SIGNAL(readyReadStandardError()),this,SLOT(printError()));
        //process->start(prog);
        //process->waitForFinished(-1);
        //delete process;
    worker.start();

    QObject::connect(&worker,SIGNAL(actualiserGUISignal()),&worker,SLOT(actualiserGUI()), Qt::QueuedConnection);
    QObject::connect(&worker,SIGNAL(stoptimerSignal()),&worker,SLOT(stopTimer()), Qt::QueuedConnection);
    QObject::connect(&worker,SIGNAL(actualiserGUISignal()),this,SLOT(actualiserProgressUi()), Qt::QueuedConnection);
    QObject::connect(&worker,SIGNAL(stoptimerSignal()),this,SLOT(stopTimerUi()), Qt::QueuedConnection);

    QObject::connect(&worker,SIGNAL(messageerrorSignal()),this,SLOT(messageError()), Qt::QueuedConnection);
    QObject::connect(this,SIGNAL(closeProgressGuiandDeleteMessageBoxSignal()),&worker,SLOT(closeProgressGui()), Qt::QueuedConnection);
    QObject::connect(this,SIGNAL(closeProgressGuiandDeleteMessageBoxSignal()),this,SLOT(deleteMessageBox()), Qt::QueuedConnection);

           //QObject::connect(&worker,SIGNAL(labelsignal(QString)),ui->label,SLOT(setText(QString)), Qt::QueuedConnection);
           //this->deleteRandomForestModel();

    QObject::connect(&worker,SIGNAL(lireFichierSignal()),this,SLOT(lireFichier()), Qt::QueuedConnection);
          //QObject::connect(ui->pushButton,SIGNAL(clicked()),&worker,SLOT(killProcess()), Qt::QueuedConnection);

    worker.timer = new QTimer(&worker);
    QObject::connect(worker.timer,SIGNAL(timeout()),&worker,SLOT(actualiserTimer()), Qt::QueuedConnection);
    QObject::connect(timerUi,SIGNAL(timeout()),this,SLOT(actualiserTimer()), Qt::QueuedConnection);
}

void MainWindow::createRandomforestModelFile()
{
    //QFile::copy(":/files/model_rf_656000_linux.model","./model_rf_656000_linux.model");
}
void MainWindow::deleteRandomForestModel()
{
    //QFile file("./model_rf_656000_linux.model");
    //file.setPermissions(QFile::WriteOther);
    //file.remove();
}
void MainWindow::lireFichier()
{
        std::ifstream file( "error_RF.txt" );
        file.peek() == std::ifstream::traits_type::eof();

        std::ifstream in( "resultat_RF.txt" );
        in.peek() == std::ifstream::traits_type::eof();
        if (in.peek()==-1) //if the file resultat is empty
        {
            // file is empty
        }
        else //if the file resultat is not empty
        {
        if(file.peek()==-1) //and if file error is empty
        {
        //in.open("resultat_RF.txt");
        std::string ch;
        in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
        in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;in>>ch;
        in>>ch;in>>ch;in>>ch;
        in>>ch;
        QString classString(ch.c_str());
        classString=classString.split(":").at(1);
        in>>ch;
        QString probaString(ch.c_str());
        probaString=probaString.split(",").at(0);
        if(probaString.size()>1)
        {
            probaString.replace("*"," ");

        }
        ui->probaLabel->setText("Probability: "+probaString);
        ui->classLabel->setText("Class: "+classString);
        ui->probaLabel->setStyleSheet("QLabel { color : red; font : bold;}");
        ui->classLabel->setStyleSheet("QLabel { color : red; font : bold;}");
        worker.progressBar->close();
        }
        }
}
void MainWindow::actualiserProgressUi()
{
    timerUi->start();
    timerUi->setInterval(4000);
    secondUi = 0;
    ui->progressBar->setMinimum(1);
    ui->progressBar->setMaximum(100);
    //connect(timerUi, SIGNAL(timeout()), this, SLOT(actualiserTimer()));
}
void MainWindow::actualiserTimer()
{
    secondUi++;
    //QString string= QString("Valeur %1").arg(secondUi);
    ui->progressBar->setValue(secondUi+1);
}
void MainWindow::stopTimerUi()
{
    timerUi->stop();
    delete timerUi;
    timerUi = new QTimer(this);
}
void MainWindow::messageError()
{
    ui->newPatientPushButton->setEnabled(true);
    ui->annRadioButton->setEnabled(true);
    ui->svmRadioButton->setEnabled(true);
    ui->naiveBayesRadioButton->setEnabled(true);
    ui->id3RadioButton->setEnabled(true);
    //QMessageBox m_error;
    std::ifstream file( "error_RF.txt" );
    file.peek() == std::ifstream::traits_type::eof();
    if (file.peek()==-1)
    {
        // file is empty
    }
    else
    {
    std::string line;
    std::string text;
    while(std::getline(file, line))
            {
                text = text + line + "\n";
            }
    QString ligneQString(text.c_str());
    m_error->setText(ligneQString);
    m_error->setWindowTitle("Error");
    m_error->setIcon(QMessageBox::Warning);
    m_error->show();
    if(m_error->exec() == QMessageBox::Ok || m_error->exec() == QMessageBox::Close){ //qDebug() << "Yes was clicked";
    emit closeProgressGuiandDeleteMessageBoxSignal();}
    }
}

void MainWindow::deleteMessageBox()
{
    delete m_error;
    m_error = new QMessageBox(this);
}













//for getting a random number 0,1,2,3,4,5,6 (for injuries) with a probability (in a way to have more number 0 and 1)
//Here we want 3, 2, 1, 0 to have a distribution of 5%, 20%, 30%, 45%.
int myrand()
{
    double val = (double)rand() / RAND_MAX; int random;
    if (val < 0.01)       //  1%
        return random = 6;
    if (val < 0.02)       //  2%
        return random = 5;
    if (val < 0.03)       //  3%
        return random = 4;
    if (val < 0.05)       //  5%
        return random = 3;
    else if (val < 0.25)  //  5% + 20%
        return random = 2;
    else if (val < 0.55)  //  5% + 20% + 30%
        return random = 1;
    else
        return random = 0;
}

void MainWindow::generateNewPatient()
{
    int min=0,max=100;
    ui->ageSpinBox->setValue(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));
    min=0; max=6;

    ui->abdomenComboBox->setCurrentIndex(myrand());
    ui->faceComboBox->setCurrentIndex(myrand());
    ui->headComboBox->setCurrentIndex(myrand());
    ui->neckComboBox->setCurrentIndex(myrand());
    ui->thoraxComboBox->setCurrentIndex(myrand());
    ui->spineComboBox->setCurrentIndex(myrand());
    ui->upperExtremityComboBox->setCurrentIndex(myrand());
    ui->lowerExtremityComboBox->setCurrentIndex(myrand());
    ui->externalComboBox->setCurrentIndex(myrand());

    min=0; max=1;
    ui->genderComboBox->setCurrentIndex(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));
    min=0; max=1;
    ui->typeOfInjuryComboBox->setCurrentIndex(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));

    min=0; max=3;
    ui->eyeMouvementComboBox->setCurrentIndex(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));
    min=0; max=4;
    ui->verbalResponsecomboBox->setCurrentIndex(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));
    min=0; max=5;
    ui->motorCoordinationComboBox->setCurrentIndex(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));

    min=0,max=300;
    ui->bloodPressureSpinBox->setValue(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));
    min=0,max=100;
    ui->respirationRateSpinBox->setValue(min+(int)(((float)rand()/RAND_MAX)*(max-min+1)));

}
