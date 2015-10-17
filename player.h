#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QStringList>

using namespace cv;
class Player : public QThread
{    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    VideoCapture *capture;
    Mat RGBframe;
    QImage img;
 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
 protected:
     void msleep(int ms);
 public:

    void run();
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    void loadCascade(string cascadename);
    //Load a video from memory
    bool loadVideo(string filename);
    bool loadStream(string filename1);
    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
    //set video properties
    void setCurrentFrame( int frameNumber);
    //Get video properties
    double getFrameRate();
    double getCurrentFrame();
    double getNumberOfFrames();
 private:
     QSqlDatabase mydb;
 private slots:
      void dbconnect();
      void createtable(QString fname);
      void insertdata(QString time, QString objects);
      void closecon();
      //void selectdata();

};
#endif // VIDEOPLAYER_H
