#include "weightchart.h"

#include "sql/SqlHelper.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartMarkerAttributes>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartGridAttributes>

using namespace KDChart;

WeightChart::WeightChart(QWidget *parent) :
    Widget(parent)
{
    const int year = QDate::currentDate().year();
    mStartDate = QDate(year, 1, 1); /* 1st january */
    mEndDate = QDate(year, 12, 31); /* 31 december */

    xAxis = new CartesianAxis(lineDiagram());
    yAxis = new CartesianAxis (lineDiagram());
    xAxis->setPosition (CartesianAxis::Bottom);
    yAxis->setPosition (CartesianAxis::Left);
    xAxis->setTitleText (tr("Date"));
    yAxis->setTitleText (tr("Masse (Kg)"));


    lineDiagram()->addAxis(xAxis);
    lineDiagram()->addAxis(yAxis);

    retrieveDatas();

    addLegend(Position::East);


    DataValueAttributes dva(lineDiagram()->dataValueAttributes(0));
    MarkerAttributes markers(dva.markerAttributes());
    markers.setMarkerStyle(MarkerAttributes::MarkerDiamond);
    markers.setMarkerSize(QSizeF(7, 7));
    markers.setVisible(true);
    dva.setMarkerAttributes(markers);
    dva.setVisible(true);
    lineDiagram()->setDataValueAttributes(0, dva);

    setGlobalLeading( 20, 20, 20, 20 );

    CartesianCoordinatePlane *plane = static_cast<CartesianCoordinatePlane *>
            (lineDiagram()->coordinatePlane());
    GridAttributes attributes(plane->gridAttributes(Qt::Vertical));
    attributes.setGridVisible(true);
    attributes.setSubGridVisible(true);
    plane->setGridAttributes(Qt::Vertical, attributes);
}

void WeightChart::setStartDate(const QDate &date)
{
    Q_ASSERT(date <= mEndDate);
    mStartDate = date;
    retrieveDatas();
}

void WeightChart::setEndDate(const QDate &date)
{
    Q_ASSERT(date >= mStartDate);
    mEndDate = date;
    retrieveDatas();
}

QDate WeightChart::startDate() const
{
    return mStartDate;
}

QDate WeightChart::endDate() const
{
    return mEndDate;
}

void WeightChart::retrieveDatas()
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT id_mensuration, weight, date FROM mensuration "
                  "WHERE date>=:startDate AND date<=:endDate");
    query.bindValue(":startDate", mStartDate);
    query.bindValue(":endDate", mEndDate);
    if (query.exec())
    {
        QStringList axisLabels;
        QStringList shortAxisLabels;
        QVector<double> values;
        while (query.next())
        {
            const double weight = query.value(1).toDouble();
            const QDateTime date = query.value(2).toDateTime();
            axisLabels << date.toString(Qt::SystemLocaleLongDate);
            shortAxisLabels << date.toString(Qt::SystemLocaleShortDate);
            values << weight;
        }

        setDataset(0, values, tr("Masse (Kg)"));
        xAxis->setLabels(axisLabels);
        xAxis->setShortLabels(shortAxisLabels);

    }
    else
    {
        qDebug() << Q_FUNC_INFO << query.lastError();
    }

}
