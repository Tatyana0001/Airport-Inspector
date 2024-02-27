#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include "database.h"
#include "dbdata.h"
#include "statistic.h"
#include <QDate>
#include <QMap>

#define MIN_DATE QDate(2016, 8, 15)
#define MAX_DATE QDate(2017, 9, 14)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ScreenDataFromDB(QSqlQueryModel *model);
    void ReceiveStatusConnectionToDB(bool status);
private slots:
    void on_act_addData_triggered();
    void on_act_connect_triggered();
    void on_pb_clear_clicked();
    void AirportsFromDB(QSqlQueryModel *model);
    void AirportsListFromDB(QSqlQueryModel *model);
    void MonthDataFromDB(QSqlQueryModel *model);
    void YearDataFromDB(QSqlQueryModel *model);
    void CloseStatistic();
    void on_pb_airports_clicked();
    void on_pb_arrival_clicked();
    void on_pb_departure_clicked();

    void on_pb_statistic_clicked();

private:

    QVector<QString> dataForConnect; //Данные для подключения к БД.

    Ui::MainWindow *ui;
    DbData *dataDb;
    DataBase* dataBase;

    Statistics* statistic;
    QMessageBox* msg;
    QMap<QString, QString> airports;
    QString request;
    QString name;

};
#endif // MAINWINDOW_H
