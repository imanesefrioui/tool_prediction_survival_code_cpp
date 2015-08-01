#include "mainwindow.h"
#include <QApplication>
#include <time.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    MainWindow w;
    w.show();
    //system("java -jar \"./weka.jar\"");
    //system("java -cp ./weka.jar weka.classifiers.trees.RandomForest -T testFile.csv -l model_rf_656000_linux.model -distribution -p 0 1>resultat_RF.txt 2>error_RF.txt");

    return a.exec();
}
