#ifndef REPORT_H
#define REPORT_H

#include <QMainWindow>

namespace Ui {
class report;
}

class report : public QMainWindow
{
    Q_OBJECT

public:
    explicit report(QWidget *parent = 0);
    ~report();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::report *ui;
};

#endif // REPORT_H
