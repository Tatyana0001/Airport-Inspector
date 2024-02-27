#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>


#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};


class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request, QTableView *view);
    void RequestArrival(QString request, QTableView *view);
    void RequestDeparture(QString request, QTableView *view);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void Year_statistic(const QString &airportCode);
    void Month_statistic(const QString &airportCode);

signals:

   void sig_SendDataFromDB(QSqlQueryModel *query_model);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError err);
   void sig_SendFlight(QSqlQueryModel *query_model);
   void sig_Airports(QSqlQueryModel *airport_model);
   void sig_Airports_list(QSqlQueryModel *airport_model);
   void sig_statistic_month(QSqlQueryModel *statistic_model);
   void sig_statistic_year(QSqlQueryModel *statistic_model);

private:

    QSqlDatabase* dataBase;
    QSqlQueryModel* query_model;
    QSqlQueryModel* airport_model;
    QSqlQueryModel* statistic_model;

};

#endif // DATABASE_H
