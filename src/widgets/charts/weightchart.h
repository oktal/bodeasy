#ifndef WEIGHTPLOT_H
#define WEIGHTPLOT_H

#include <QDate>
#include <KDChart/KDChartWidget>
#include <QStringList>

namespace KDChart
{
class CartesianAxis;
}

class WeightChart : public KDChart::Widget
{
    Q_OBJECT
public:
    explicit WeightChart(QWidget *parent = 0);

    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);

    QDate startDate() const;
    QDate endDate() const;

private:
    void retrieveDatas();
    void setAxisLabels();

    QDate mStartDate;
    QDate mEndDate;

    KDChart::CartesianAxis *xAxis;
    KDChart::CartesianAxis *yAxis;

};

#endif // WEIGHTPLOT_H
