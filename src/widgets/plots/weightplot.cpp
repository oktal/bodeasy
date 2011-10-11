#include "weightplot.h"

#include <qwt_plot_curve.h>

WeightPlot::WeightPlot(QWidget *parent) :
    QwtPlot(parent),
    mWeightCurve(new QwtPlotCurve)
{
    const int year = QDate::currentDate().year();
    mStartDate = QDate(year, 1, 1); /* 1st january */
    mEndDate = QDate(year, 12, 31); /* 31 december */


}

void WeightPlot::setStartDate(const QDate &date)
{
    Q_ASSERT(date <= mEndDate);
    mStartDate = date;
    replot();
}

void WeightPlot::setEndDate(const QDate &date)
{
    Q_ASSERT(date >= mStartDate);
    mEndDate = date;
    replot();
}

QDate WeightPlot::startDate() const
{
    return mStartDate;
}

QDate WeightPlot::endDate() const
{
    return mEndDate;
}
