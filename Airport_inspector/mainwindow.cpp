#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Исходное состояние виджетов
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_airports->setEnabled(false);
    ui->de_date->setMinimumDate(MIN_DATE);
    ui->de_date->setMaximumDate(MAX_DATE);

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);
    statistic = new Statistics();

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_Airports, this, &MainWindow::AirportsFromDB);
    connect(dataBase, &DataBase::sig_Airports_list, this, &MainWindow::AirportsListFromDB);
    connect(dataBase, &DataBase::sig_SendFlight, this, &MainWindow::ScreenDataFromDB);
    connect(dataBase, &DataBase::sig_statistic_month, this, &MainWindow::MonthDataFromDB);
    connect(dataBase, &DataBase::sig_statistic_year, this, &MainWindow::YearDataFromDB);
    connect(statistic, &Statistics::sig_closeStatistic, this, &MainWindow::CloseStatistic);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно.
    dataDb->show();
}

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_airports->setEnabled(false);
    }

}


void MainWindow::ScreenDataFromDB(QSqlQueryModel *model)
{
    ui->tv_result->setModel(model);
    ui->tv_result->showColumn(0);
    ui->tv_result->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_airports->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

void MainWindow::on_pb_clear_clicked()
{
    ui->tv_result->setModel(0);
}

void MainWindow::AirportsFromDB(QSqlQueryModel *model)
{
    ui->cb_airport->setModel(model);
    ui->cb_airport->setModelColumn(1);
}

void MainWindow::AirportsListFromDB(QSqlQueryModel *model)
{
    for(int i = 0; i<model->rowCount(); i++){
        airports[model->data(model->index(i,1)).toString()] = model->data(model->index(i,0)).toString();
    }
}

void MainWindow::MonthDataFromDB(QSqlQueryModel *model)
{
    QMultiMap<int, int> month_data;
        for (int i = 0; i < model->rowCount(); ++i) {
            month_data.insert(model->data(model->index(i, 1)).toInt(),
                        model->data(model->index(i, 0)).toInt());
        }
        statistic->setMonthData(month_data);
}

void MainWindow::YearDataFromDB(QSqlQueryModel *model)
{
    QVector<int> year_data;
    for(int i = 0; i<model->rowCount(); i++){
        year_data.append(model->data(model->index(i,0)).toInt());
    }
    statistic->setYearData(year_data);
}

void MainWindow::CloseStatistic()
{
    ui->centralwidget->setEnabled(true);
    statistic->close();
}

void MainWindow::on_pb_airports_clicked()
{
    request = "SELECT airport_name->>'ru', airport_code FROM bookings.airports_data ORDER BY airport_name";
    auto req = [&]{dataBase->RequestToDB(request, ui->tv_result);};
    QtConcurrent::run(req);
}


void MainWindow::on_pb_arrival_clicked()
{
    QString Airport_code = ui->cb_airport->currentText();
    QString date = ui->de_date->text();
    QString day, month, year;
    for(int i = 0; i<date.size(); i++){
        if(i<2){day+=date[i];}
        if(i>2 && i<5) {month+=date[i];}
        if(i>5){year+=date[i];}
    }
    QString convert_date = year + "-" + month + "-" + day;;

    request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as name from bookings.flights f "
    "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
    " where (f.arrival_airport  = '" + Airport_code + "' AND f.scheduled_arrival::date = date('" + convert_date +
    "')) ORDER BY name";
    auto req = [&]{dataBase->RequestArrival(request, ui->tv_result);};
    QtConcurrent::run(req);
}


void MainWindow::on_pb_departure_clicked()
{
    QString Airport_code = ui->cb_airport->currentText();
    QString date = ui->de_date->text();
    QString day, month, year;
    for(int i = 0; i<date.size(); i++){
        if(i<2){day+=date[i];}
        if(i>2 && i<5) {month+=date[i];}
        if(i>5){year+=date[i];}
    }
    QString convert_date = year + "-" + month + "-" + day;;

    request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as name from bookings.flights f "
    "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
    " where (f.departure_airport  = '" + Airport_code + "' AND f.scheduled_arrival::date = date('" + convert_date +
    "')) ORDER BY name";
    auto req = [&]{dataBase->RequestDeparture(request, ui->tv_result);};
    QtConcurrent::run(req);
}


void MainWindow::on_pb_statistic_clicked()
{
    ui->centralwidget->setEnabled(false);
    QString Airport_code = ui->cb_airport->currentText();
    dataBase->Month_statistic(Airport_code);
    dataBase->Year_statistic(Airport_code);
    name = airports.value(ui->cb_airport->currentText());
    statistic->setAirportName(name);
    statistic->show();
}

