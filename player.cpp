#include "player.h"
#include <QString>
#include <QtSql>
#include <QtDebug>
#include <mainwindow.h>
#include <comman.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QImage>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function Headers
void detectAndDisplay(Mat frame);
//int global;
QString f_name;
double timex=0;

Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
    //connect to the data base
     QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
     mydb.setDatabaseName("/home/kapila/final.db");
     mydb.open();

}

bool Player::loadStream(string filename1) {

    capture  =  new cv::VideoCapture(filename1);
    //create table
    QSqlQuery qry;
    qry.prepare( "CREATE TABLE IF NOT EXISTS '"+neededWord2+"' (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, time VARCHAR(30), objects VARCHAR(30),comments VARCHAR(50))" );
    if( !qry.exec() ){
     qDebug() << qry.lastError();
    }
    else{
     qDebug() << "Table created!";
     /*QSqlQuery qryx;
     qryx.prepare("INSERT IN TO vlist (vname) VALUES '"+neededWord2+"'WHERE EXIST (SELECT *FROM vlist WHERE vname='"+neededWord2+"')");
     qryx.exec();*/
     }
    if (capture->isOpened())
    {
        frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

bool Player::loadVideo(string filename1) {

    capture  =  new cv::VideoCapture(filename1);
    QString str = QString::fromUtf8(filename1.c_str());
    QStringList pieces = str.split( "/" );
    QString neededWord = pieces.value( pieces.length() - 1 );
    QStringList pieces1 = neededWord.split( "." );
    neededWord2 = pieces1.value( pieces1.length()-2);
   /* //create table
    QSqlQuery qry1;
    QString mycode;
    qry1.prepare("INSERT INTO vname (vname) VALUES ('"+neededWord2+"')");
    QSqlQuery querylite;
    querylite.prepare("SELECT * FROM vlist WHERE vname='"+neededWord2+"'");
    if( !querylite.exec() )
     qDebug() << querylite.lastError();
    else{
        while (querylite.next()) //While you have records
        {
        mycode = querylite.value(querylite.record().indexOf("code")).toString();  //Get the data from the query
       qDebug() << mycode ; //set the data to the label
        }
     qDebug() << "Table created!";}*/
    f_name=neededWord2;
    //create table
    QSqlQuery qry;
    qry.prepare( "CREATE TABLE IF NOT EXISTS '"+neededWord2+"' (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, time VARCHAR(30), objects VARCHAR(30),comments VARCHAR(50))" );
    if( !qry.exec() ){
     qDebug() << qry.lastError();}
    else{
     qDebug() << "Table created!";
     QSqlQuery qrya;
     qrya.prepare("INSERT INTO vlist (vname) VALUES ('"+neededWord2+"')");
     if(!qrya.exec()){
         qDebug() <<qrya.lastError();}
     else{
         qDebug() << neededWord2;}
    }
    if (capture->isOpened())
    {
        frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

//string face_cascade_name = "/home/kapila/opencv-2.4.11/data/hogcascades/hogcascade_pedestrians.xml";
//string face_cascade_name="";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
//string videopath="/home/kapila/Desktop/123.mp4";
string videopath;
string filename1;


void Player::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

//QSqlDatabase mydb;
// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{

    float t = 0.0;
    t = (double)cvGetTickCount();

    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    stringstream sstm;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

// Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    //face_cascade.detectSingleScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

// Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;

    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element

    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element

   // mydb = QSqlDatabase::addDatabase("QSQLITE");
   // mydb.setDatabaseName("/home/kapila/scoredb.bd ");
   // mydb.open();
    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

    {   QSqlQuery qry;
        int cont=ic;
        QString AsString = QString::number(cont+1);

        //ui->textEdit->setText(AsString );
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)
        //printf("x =%d,y = %d ",roi_c.x,roi_c.y);
        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }

        crop = frame(roi_b);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

        // Form a filename
        /*filename = "";
        stringstream ssfn;
        ssfn << filenumber << ".png";
        filename = ssfn.str();
        filenumber++;*/

        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);

        t = (double)cvGetTickCount() + t;
        timex=timex+t;
        QString tString = QString::number(timex);
        printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.));
        //insertdata(tString, "VIDEO");
        global_count=AsString;
        global_time=tString;
        qry.prepare( "INSERT INTO '"+neededWord2+"' (time,objects) VALUES ('"+tString+"', '"+AsString+"')");
          if( !qry.exec() )
            qDebug() << qry.lastError();
          else
            qDebug( "Inserted!" );
        //QSqlQuery qry;
        //qry.prepare("INSERT INTO "+str+" (time) VALUES('"+tString+"'')");
        /*QSqlQuery qry;
        qry.prepare("INSERT INTO "+str+"(vtime) VALUES('"+tString+"')");
        if(qry.exec()){
            printf("data added");
        }else{
            printf("can't added");
        }
*/



    }

    putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);

    //mydb.removeDatabase(QSqlDatabase::defaultConnection);
}

string name="haarcascade_frontalface_alt.xml";

void Player::loadCascade(string cascadename)
{
//string facecascade="haarcascade_frontalface_alt.xml";

//string face_cascade_name="~/New3/cascades/haar/";
//face_cascade_name.append(cascadename);

//face_cascade.load(face_cascade_name);
    face_cascade.load(cascadename);
}



void Player::run()
{
    //string facecascade="haarcascade_frontalface_alt.xml";
    //string face_cascade_name="/home/kapila/opencv-2.4.11/data/hogcascades/";
    //face_cascade_name.append(facecascade);
    int delay = (1000/frameRate);
    while(!stop){
        if (!capture->read(frame))
        {
            stop = true;
        }
        if (frame.channels()== 3){
            if (!frame.empty())
            {
                detectAndDisplay(frame);
            }
            else
            {
                break;
            }
            /*if (!face_cascade.load(face_cascade_name))
            {
                printf("--(!)Error loading\n");
            };*/
            //face_cascade.load(face_cascade_name);
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        emit processedImage(img);
        this->msleep(delay);
    }
}
//database
void Player::dbconnect()
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("/home/kapila/final.db");
    mydb.open();
}

void Player::createtable(QString fname){
    QSqlQuery qry;
    qry.prepare( "CREATE TABLE IF NOT EXISTS "+fname+" (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, time VARCHAR(30), objects VARCHAR(30))" );
      if( !qry.exec() )
        qDebug() << qry.lastError();
      else
        qDebug() << "Table created!";
}

void Player::insertdata(QString time, QString objects){
     QSqlQuery qry;
     qry.prepare( "INSERT INTO names (time,objects) VALUES ('"+time+"', '"+objects+"')");
       if( !qry.exec() )
         qDebug() << qry.lastError();
       else
         qDebug( "Inserted!" );
}

void Player::closecon()
{
    mydb.close();
}
//end db functions

Player::~Player()
{
    mutex.lock();
    stop = true;
    capture->release();
    delete capture;
    condition.wakeOne();
    mutex.unlock();
    wait();
    mydb.close();
}
void Player::Stop()
{
    stop = true;
}
void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
bool Player::isStopped() const{
    return this->stop;
}

double Player::getCurrentFrame(){

    return capture->get(CV_CAP_PROP_POS_FRAMES);
}

double Player::getNumberOfFrames(){

    return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

double Player::getFrameRate(){
    return frameRate;
}

void Player::setCurrentFrame( int frameNumber )
{
    capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);
}

/*void Player::creatable(QString tname)
{
    QSqlQuery qry;
    qry.prepare("CREATE TABLE "+tname+"(vtime TEXT,comment TEXT)");
    qry.exec();
}

void Player::insertdata(QString time, QString tname)
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO "+tname+"(vtime) VALUES('"+time+"')");
    if(qry.exec()){
        printf("data added");
    }else{
        printf("can't added");
    }

}*/

//data base connecting ,inserting , close functions

