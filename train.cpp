#include "train.h"
#include "ui_train.h"
#include <QMessageBox>
#include <QFileDialog>
train::train(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::train)
{
    ui->setupUi(this);
}

train::~train()
{
    delete ui;
}

void train::on_pushButton_3_pressed()
{
    if(ui->Radio_hog->isChecked()){
        QString txt = ui->textEdit->toPlainText();
        QString command ;
        QString gen =" genfiles/";
        QString ext =".dat";
        QString finaltxt = "./opencvhogtrainer";
        command += "gnome-terminal -x sh -c 'cd ..; cd Desktop ;cd Score_detector1;cd samp_hog;";
        command +="g++ `pkg-config --cflags opencv` -c -g -MMD -MP -MF main.o.d -o main.o main.cpp;";
        command +="gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_learn.o.d -o svmlight/svm_learn.o svmlight/svm_learn.c;";
        command +="gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_hideo.o.d -o svmlight/svm_hideo.o svmlight/svm_hideo.c;";
        command +="gcc -c -g `pkg-config --cflags opencv` -MMD -MP -MF svmlight/svm_common.o.d -o svmlight/svm_common.o svmlight/svm_common.c;";
        command +="g++ `pkg-config --cflags opencv` -o opencvhogtrainer main.o svmlight/svm_learn.o svmlight/svm_hideo.o svmlight/svm_common.o `pkg-config --libs opencv`;";
        command +=finaltxt;
        command += gen;
        command += "feature";
        command += txt;
        command += ext;
        command += gen;
        command += "descriptor";
        command += txt;
        command += ext;
        command += ";'" ;
        system(command.toLatin1().data());
        QMessageBox msgBox;
        msgBox.setText(command);
        msgBox.exec();
    }if(ui->radio_haar->isChecked()){
        QMessageBox msgBox;
        msgBox.setText("Haar selected");
        msgBox.exec();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Please select Algorithm");
        msgBox.exec();
    }
}

void train::on_pushButton_pressed()
{
    QString path_pos = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath());
    QMessageBox msg;
    msg.setText("Are You  going to get positivie data from "+path_pos+" ?");
    msg.exec();
}

void train::on_pushButton_2_pressed()
{
    QString path_neg = QFileDialog::getOpenFileName(this,tr("Find Files"), QDir::currentPath());
    QMessageBox msg;
    msg.setText("Are You  going to get positivie data from "+path_neg+" ?");
    msg.exec();
}
