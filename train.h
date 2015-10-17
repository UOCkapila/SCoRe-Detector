#ifndef TRAIN_H
#define TRAIN_H

#include <QMainWindow>

namespace Ui {
class train;
}

class train : public QMainWindow
{
    Q_OBJECT

public:
    explicit train(QWidget *parent = 0);
    ~train();

private slots:
    void on_pushButton_3_pressed();

    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

private:
    Ui::train *ui;
};

#endif // TRAIN_H
