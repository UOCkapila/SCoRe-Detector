#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include <QTime>
#include <QSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <report.h>
#include <train.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updatePlayerUI(QImage img);
    QString getFormattedTime(int timeInSeconds);

    void on_btnLoadVideo_clicked();

    void on_btnPlay_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderPressed();

    void on_comboBox_windowIconTextChanged(const QString &iconText);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_btncascade_pressed();

    void on_comboBox_currentIndexChanged(const QString &arg1);
    void getreport();
    void dbconnect();
    void closecon();
    void createtable();
    void insertdata();
    void selectdata();


    void on_pushButton_pressed();

    void on_btnLoadVideo_pressed();

    void on_pushButton_clicked();

    void on_pushButton_2_pressed();

    void on_btnLoadStream_clicked();

    void on_pushButton_3_pressed();

private:

    Player* myPlayer;
    QSqlDatabase mydb;
    report *rep;
    train *tra;
};

#endif // MAINWINDOW_H
