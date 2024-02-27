#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    query_model = new QSqlQueryModel();
    airport_model = new QSqlQueryModel();
    statistic_model = new QSqlQueryModel();
}

DataBase::~DataBase()
{
    delete dataBase;
    delete query_model;
    delete airport_model;
    delete statistic_model;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);
    airport_model->setQuery("SELECT airport_name->>'ru', airport_code FROM bookings.airports_data ORDER BY airport_name", *dataBase);
    emit sig_Airports(airport_model);
    emit sig_Airports_list(airport_model);
}

void DataBase::Year_statistic(const QString &airportCode)
{
    if(!dataBase->isOpen()){
        emit sig_SendStatusConnection(false);
    }
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) "
                      "as Month from bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "and scheduled_departure::date <= date('2017-08-31')) and "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "group by Month";
    statistic_model->setQuery(request, *dataBase);
    emit sig_statistic_year(statistic_model);
}

void DataBase::Month_statistic(const QString &airportCode)
{
    if(!dataBase->isOpen()){
        emit sig_SendStatusConnection(false);
    }
    QString request = "SELECT count(flight_no), date_part('month', date_trunc('day', scheduled_departure)),"
                      "date_trunc('day', scheduled_departure) as Day FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "and scheduled_departure::date <= date('2017-08-31')) and "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "group by Day";
    statistic_model->setQuery(request, *dataBase);
    emit sig_statistic_month(statistic_model);
}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request, QTableView *view)
{
    if(!dataBase->isOpen()){
        emit sig_SendStatusConnection(false);
    }
        query_model->setQuery(request, *dataBase);
        query_model->setHeaderData(0, Qt::Horizontal, tr("Название аэропорта"));
        query_model->setHeaderData(1, Qt::Horizontal, tr("Код"));
        emit sig_SendDataFromDB(query_model);
}

void DataBase::RequestArrival(QString request, QTableView *view)
{
    if(!dataBase->isOpen()){
        emit sig_SendStatusConnection(false);
    }
        query_model->setQuery(request, *dataBase);
        query_model->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        query_model->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
        query_model->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
        emit sig_SendFlight(query_model);
}

void DataBase::RequestDeparture(QString request, QTableView *view)
{
    if(!dataBase->isOpen()){
        emit sig_SendStatusConnection(false);
    }
        query_model->setQuery(request, *dataBase);
        query_model->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        query_model->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
        query_model->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
        emit sig_SendFlight(query_model);
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError(){
    return dataBase->lastError();
}
