#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QtDebug>
#include <comman.h>


using namespace std;
QString global_time;
QString global_count;
string cascade;
QString filename;
QString neededWord2;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myPlayer = new Player();
    myPlayer->loadCascade("haarcascadefrontalface.xml");
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    ui->setupUi(this);
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::Background,Qt::black);
    ui->label->setPalette(palette);
    ui->label->setAutoFillBackground(true);


    if (myPlayer->isStopped())
    {
        ui->comboBox->setEnabled(true);
    }else
    {
        ui->comboBox->setEnabled(false);
    }
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->horizontalSlider->setValue(myPlayer->getCurrentFrame());
        ui->label_2->setText( getFormattedTime( (int)myPlayer->getCurrentFrame()/(int)myPlayer->getFrameRate()) );
        //ui->textEdit->setText(global);
        ui->label_object->setText(global_count);
        ui->label_time->setText(global_time);

    }
}

MainWindow::~MainWindow()
{
    delete myPlayer;
    delete ui;
}


QString MainWindow::getFormattedTime(int timeInSeconds){
    int seconds = (int) (timeInSeconds) % 60 ;
    int minutes = (int) ((timeInSeconds / 60) % 60);
    int hours   = (int) ((timeInSeconds / (60*60)) % 24);

    QTime t(hours, minutes, seconds);
    if (hours == 0 )
        return t.toString("mm:ss");
    else
        return t.toString("h:mm:ss:ms");
}

void MainWindow::on_btnLoadVideo_clicked()
{
    ui->btnPlay->setEnabled(false);
    ui->label->setScaledContents(true);
    filename = QFileDialog::getOpenFileName(this,tr("Open Video"), ".",tr("Video Files (*.avi *.mpg *.mp4)"));
    QStringList pieces = filename.split( "/" );
    QString neededWord = pieces.value( pieces.length() - 1 );
    QStringList pieces1 = neededWord.split( "." );
    neededWord2 = pieces1.value( pieces1.length()-2);
    //QString filename = "/home/kapila/Desktop/123.mp4";
    if (!filename.isEmpty()){

        if (!myPlayer->loadVideo(filename.toLatin1().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }else{
            ui->btnPlay->setEnabled(true);
            ui->label_5->setText(f_name+" ready");
        }
    }
}

void MainWindow::on_btnPlay_clicked()
{

    if (myPlayer->isStopped())
    {
        ui->btncascade->setEnabled(false);
        ui->label_5->setText("Playing "+f_name);
        myPlayer->Play();
        ui->btnPlay->setText(tr("Stop"));
    }else
    {
        ui->btncascade->setEnabled(true);
        ui->label_5->setText("Pause "+f_name);
        myPlayer->Stop();
        ui->btnPlay->setText(tr("Play"));
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    myPlayer->setCurrentFrame(position);
    ui->label_2->setText( getFormattedTime( position/(int)myPlayer->getFrameRate()) );
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    myPlayer->Play();

}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    myPlayer->Play();
}

void MainWindow::on_comboBox_windowIconTextChanged(const QString &iconText)
{
    ui->label_2->setText(iconText);
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
        ui->label_2->setText(arg1);
        std::string utf8_text = arg1.toUtf8().constData();
        if(utf8_text.length()>0)
        {
            myPlayer->loadCascade(utf8_text);
        }
        else
        {
            myPlayer->loadCascade(utf8_text);
        }
}

void MainWindow::on_btncascade_pressed()
{
    filename = QFileDialog::getOpenFileName(this,tr("Open Cascade"), ".",tr("Cascade Files (*.xml *.dat)"));
    myPlayer->loadCascade(filename.toLatin1().data());
    QMessageBox msgBox;
    msgBox.setText(filename);
    msgBox.exec();
    ui->btnPlay->setEnabled(true);

}



void MainWindow::on_btnLoadVideo_pressed()
{

}

void MainWindow::on_pushButton_pressed()
{

    QString comment = ui->textEdit->toPlainText();

    QSqlQuery qry;
    qry.prepare( "UPDATE '"+neededWord2+"' SET comments = '"+comment+"' WHERE time = '"+global_time+"'" );
     if( !qry.exec() ){
       //qDebug() << qry.lastError();
         QMessageBox msg;
         msg.setText("Error");
         msg.exec();
     }else{
         ui->textEdit->clear();
         QMessageBox msg;
         msg.setText("Successfully added");
         msg.exec();
       qDebug( "Updated!" );
     }
}



void MainWindow::on_pushButton_2_pressed()
{
    rep = new report(this);
    rep->show();
}

void MainWindow::on_btnLoadStream_clicked()
{
    QString path =ui->textEditAdd->toPlainText();
    QString port=ui->edit_port->toPlainText();
    QString finalip=("http://"+path+":"+port+"/video?x.mjpeg");
    neededWord2=path;
    ui->label_5->setText("Loaded Video From"+path);
    String newpath =finalip.toUtf8().constData();
    //filename = QFileDialog::getOpenFileName(this,tr("Open Cascade"), ".",tr("Cascade Files (*.xml *.dat)"));
    myPlayer->loadStream(newpath);
    QMessageBox msgBox;
    msgBox.setText("Load Stream");
    msgBox.exec();
    ui->btnPlay->setEnabled(true);
}

void MainWindow::on_pushButton_3_pressed()
{
    tra = new train(this);
    tra->show();
}
