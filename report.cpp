#include "report.h"
#include "ui_report.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QtDebug>
#include <comman.h>


report::report(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::report)
{
    ui->setupUi(this);
   QSqlQuery get_ship_details;
    get_ship_details.prepare("SELECT time,objects,comments FROM '"+f_name+"'");
    if(get_ship_details.exec())
    {
        qDebug ()<< get_ship_details.lastError();
    } else{
        qDebug()<<(f_name);
    }
    QSqlQueryModel *model2 = new QSqlQueryModel();
    model2->setQuery(get_ship_details);
    ui->tableView->setModel(model2);
    //ui->table_controllnew_home->setModel(model2);
    ui->label->setText("Full report about "+f_name+" video");
    /*QSqlQuery allvids;
    allvids.prepare("SELECT vname FROM vlist");
    if(allvids.exec())
    {
        qDebug ()<< allvids.lastError();
    } else{
        qDebug()<<(f_name);
    }
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(allvids);
    ui->comboBox->setModel(model);*/


}

report::~report()
{
    delete ui;
    wait();
}
