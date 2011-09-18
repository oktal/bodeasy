#ifndef ACTIVITYPLOT_H
#define ACTIVITYPLOT_H

#include <QWidget>
#include <QVector>

#include <KDChart/KDChartWidget>

class ActivityChart : public KDChart::Widget
{
    Q_OBJECT
public:
    explicit ActivityChart(QWidget *parent = 0);

private:
    void retrieveDatas();

};

#endif // ACTIVITYPLOT_H
