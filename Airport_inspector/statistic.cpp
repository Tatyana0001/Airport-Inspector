#include "statistic.h"
#include "ui_statistic.h"

Statistics::Statistics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Statistics)
{
    ui->setupUi(this);
    month_chart = new QChart();
    year_chart = new QChart();

    ui->tabWidget->setTabText(0, "Месяц");
    ui->tabWidget->setTabText(1, "Год");
    month_chartView = new QChartView(month_chart);
    year_chartView = new QChartView(year_chart);
    months = {
        {1, "Январь"},
        {2, "Февраль"},
        {3, "Март"},
        {4, "Апрель"},
        {5, "Май"},
        {6, "Июнь"},
        {7, "Июль"},
        {8, "Август"},
        {9, "Сентябрь"},
        {10, "Октябрь"},
        {11, "Ноябрь"},
        {12, "Декабрь"}
    };
    for(const auto &[key, value] : months.asKeyValueRange()){
        ui->cb_month->addItem(value);
    }
    yearStatisticsLoaded = true;
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::setMonthData(const QMultiMap<int, int> data)
{
    month_data = data;
    monthStatisticsLoaded = true;
    setMonthStatistic(1);
}

void Statistics::setYearData(const QVector<int> data)
{
    year_data = data;
    setYearStatistic();
}

void Statistics::setAirportName(const QString name)
{
    ui->lb_airport->setText(name);
}

void Statistics::setMonthStatistic(const int month)
{
    month_chart->removeAllSeries();
    series = new QLineSeries(month_chart);
    int i = 0;

    for (const auto &[key, value] : month_data.asKeyValueRange()) {
        if (key == month) {
                ++i;
                series->append(i, value);
        }
    }

    month_chart->legend()->hide();
    month_chart->addSeries(series);
    if(axisX == nullptr){
        axisX = new QValueAxis(this);
        axisX->setTitleText("День");
        axisX->setLabelFormat("%.2f");
        month_chart->addAxis(axisX, Qt::AlignBottom);
    }
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
        axisX->setRange(1,31);
        axisX->setTickCount(16);
    }else if(month == 4 || month == 6 || month == 9 || month == 11){
        axisX->setRange(1,30);
        axisX->setTickCount(15);
    }else{
        axisX->setRange(1,28);
        axisX->setTickCount(15);
    }
    axisX->applyNiceNumbers();
    series->attachAxis(axisX);

    if(axisY == nullptr){
        axisY = new QValueAxis(this);
        axisY->setTitleText("Количество полетов");
        axisY->setLabelFormat("%d");
        month_chart->addAxis(axisY, Qt::AlignLeft);
    }

    axisY->setRange(0,20);
    axisY->setTickCount(6);

    series->attachAxis(axisY);

    month_chartView->setChart(month_chart);
    month_chartView->setRenderHint(QPainter::Antialiasing);
    ui->vl_month->addWidget(month_chartView);
    show();
}

void Statistics::setYearStatistic()
{
    year_chart->removeAllSeries();
    QBarSeries *yearSeries = new QBarSeries();
    QBarSet *set = new QBarSet("Месяц");
    int max = 1;
    for (const auto i : year_data){
        if(i>max){max = i;}
        set->append(i);
    }
    yearSeries->append(set);
    year_chart->addSeries(yearSeries);

    QStringList categories {"Сентябрь", "Октябрь", "Ноябрь", "Декабрь", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август"};
    if(YearAxisX == nullptr){
        YearAxisX = new QBarCategoryAxis;
        YearAxisX->append(categories);
        year_chart->addAxis(YearAxisX, Qt::AlignBottom);
    }
    yearSeries->attachAxis(YearAxisX);
    if (YearAxisY == nullptr){
        YearAxisY = new QValueAxis;
        YearAxisY->setRange(0, max);
        year_chart->addAxis(YearAxisY, Qt::AlignLeft);
    }
    yearSeries->attachAxis(YearAxisY);

    YearAxisY->setTitleText("Количество полетов");
    YearAxisY->setRange(0, max+1);
    YearAxisY->applyNiceNumbers();
    year_chartView->setChart(year_chart);
    year_chartView->setRenderHint(QPainter::Antialiasing);
    ui->hl_year->addWidget(year_chartView);
    show();
}

int Statistics::find_month(const QString &month){
    for (const auto &[key, value] : months.asKeyValueRange()) {
            if (month == value) {
                return key;
            }
    }
    return -1;
}

void Statistics::on_cb_month_currentTextChanged(const QString &arg1)
{
    if(monthStatisticsLoaded == true){
        setMonthStatistic(find_month(arg1));
    }
}


void Statistics::on_pb_finish_clicked()
{
    monthStatisticsLoaded = false;
    ui->cb_month->setCurrentText(months[1]);
    emit sig_closeStatistic();
}

