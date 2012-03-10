#ifndef WEIGHTPLOT_H
#define WEIGHTPLOT_H

#include <qwt_plot.h>

#include <QDate>

class QwtPlotCurve;

class WeightPlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit WeightPlot(QWidget *parent = 0);

    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);

    QDate startDate() const;
    QDate endDate() const;

private:
    QwtPlotCurve *mWeightCurve;

    QDate mStartDate;
    QDate mEndDate;

};

#endif // WEIGHTPLOT_H
