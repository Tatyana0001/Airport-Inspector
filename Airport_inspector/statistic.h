#ifndef STATISTICS_H
#define STATISTICS_H

///Подключаем все что нужно для графиков
#include <QLineSeries>
#include <QtCharts>
#include <QWidget>
#include <QChartView>
#include "database.h"
#include <QMultiMap>
#include <QVector>
#include <QValueAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>


namespace Ui { class Statistics; }

class Statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics();
    void setMonthData(const QMultiMap<int, int> data);
    void setYearData(const QVector<int> data);
    void setAirportName(const QString name);

private slots:
    void setMonthStatistic(const int month);
    void setYearStatistic();
    int find_month(const QString &month);
    void on_cb_month_currentTextChanged(const QString &arg1);
    void on_pb_finish_clicked();

signals:
    void sig_closeStatistic();

private:
    Ui::Statistics *ui;
    QLineSeries* series;
    QChartView* month_chartView;
    QChartView* year_chartView;
    QChart* month_chart;
    QChart* year_chart;
    QMap<int, QString> months;
    QVector<int> monthX;
    QVector<int> monthY;
    QMultiMap<int, int> month_data;
    QVector<int> year_data;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QBarCategoryAxis* YearAxisX = nullptr;
    QValueAxis* YearAxisY = nullptr;
    bool yearStatisticsLoaded = false;
    bool monthStatisticsLoaded = false;
};
#endif // STATISTICS_H
