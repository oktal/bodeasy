#ifndef ACTIVITYPLOT_H
#define ACTIVITYPLOT_H

#include <QWidget>
#include <QVector>

#include <KDChart/KDChartWidget>

#include <QMap>
#include <QVector>

class ActivityChart : public KDChart::Widget
{
    Q_OBJECT
public:
    explicit ActivityChart(QWidget *parent = 0);

    void resetColors();

private:
    void retrieveDatas();

    QMap<int, QVector<double> > mValues; // year, count

};

#endif // ACTIVITYPLOT_H
