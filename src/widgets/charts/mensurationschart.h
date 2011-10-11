#ifndef MENSURATIONSCHART_H
#define MENSURATIONSCHART_H

#include <KDChart/KDChartWidget>

#include <QDate>

namespace KDChart
{
class CartesianAxis;
}

class MensurationsChart : public KDChart::Widget
{
    Q_OBJECT
public:
    explicit MensurationsChart(QWidget *parent = 0);

    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);
    QDate startDate() const { return mStartDate; }
    QDate endDate() const { return mEndDate; }

private:
    void retrieveDatas();

    QDate mStartDate;
    QDate mEndDate;

    KDChart::CartesianAxis *xAxis;
    KDChart::CartesianAxis *yAxis;
};

#endif // MENSURATIONSCHART_H
