#ifndef ACTIVITYPLOT_H
#define ACTIVITYPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QWidget>
#include <QVector>

class ActivityPlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit ActivityPlot(QWidget *parent = 0);

private:
    void retrieveDatas();

    QVector<QwtPlotCurve *> mCurves;

};

#endif // ACTIVITYPLOT_H
