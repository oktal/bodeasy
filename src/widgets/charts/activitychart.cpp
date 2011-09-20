#include "activitychart.h"

#include <KDChart/KDChartWidget>
#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartMarkerAttributes>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartBackgroundAttributes>
#include <KDChart/KDChartGridAttributes>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <QPen>
#include <QColor>
#include <QVector>
#include <QFont>
#include <QDate>
#include <QSqlQuery>
#include <QSizeF>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

#include "sql/SqlHelper.h"

using namespace KDChart;

/* colors for different curves */
static const QColor curvesColor[] =
{
    QColor(237, 28, 38, 180),
    QColor(128, 128, 255, 180),
    QColor(157, 79, 0, 180),
    QColor(132, 0, 132, 180),
    QColor(128, 128, 64, 180),
    QColor(128, 0, 0, 180)
};

const size_t colorsCount = sizeof curvesColor / sizeof *curvesColor;

static size_t colorIndex = 0;

ActivityChart::ActivityChart(QWidget *parent) :
    Widget(parent)
{
    retrieveDatas();

    addLegend(Position::East);

    CartesianAxis *xAxis = new CartesianAxis(lineDiagram());
    CartesianAxis *yAxis = new CartesianAxis (lineDiagram());
    xAxis->setPosition (CartesianAxis::Bottom);
    yAxis->setPosition (CartesianAxis::Left);
    xAxis->setTitleText (tr("Mois"));
    yAxis->setTitleText (trUtf8("Séances effectuées"));

    QStringList shortMonthsName;
    QStringList monthsName;

    const int monthsCount = 12;
    for (int i = 0; i < monthsCount; ++i)
    {
        QString shortMonthName = locale().monthName(i + 1, QLocale::ShortFormat);
        shortMonthName[0] = shortMonthName[0].toTitleCase();
        shortMonthsName << shortMonthName;
        QString monthName = locale().monthName(i + 1, QLocale::LongFormat);
        monthName[0] = monthName[0].toTitleCase();
        monthsName << monthName;
    }

    xAxis->setLabels(monthsName);
    xAxis->setShortLabels(shortMonthsName);

    lineDiagram()->addAxis(xAxis);
    lineDiagram()->addAxis(yAxis);


    // show data values
    DataValueAttributes a(lineDiagram()->dataValueAttributes());
    a.setVisible(true);
    lineDiagram()->setDataValueAttributes(a);


    setGlobalLeading( 20, 20, 20, 20 );

    CartesianCoordinatePlane *plane = static_cast<CartesianCoordinatePlane *>
            (lineDiagram()->coordinatePlane());
    GridAttributes attributes(plane->gridAttributes(Qt::Vertical));
    attributes.setGridStepWidth(1.0);
    attributes.setGridSubStepWidth( 0.5 );
    attributes.setGridVisible(  true );
    attributes.setSubGridVisible( true );
    plane->setGridAttributes(Qt::Vertical, attributes);

}

void ActivityChart::retrieveDatas()
{
    const QString sql = "SELECT strftime('%Y', date) AS year, strftime('%m', date) AS month, COUNT(date) AS count FROM session_made GROUP BY strftime('%Y%m', date)";
    QSqlQuery query = SqlHelper::query();
    int curveNumber = 0;
    if (query.exec(sql))
    {
        QMap<int, QVector<double> > values; // year, count
        while (query.next())
        {
            const int year = query.value(0).toInt();
            const int month = query.value(1).toInt();
            const int count = query.value(2).toInt();
            QVector<double>& vector = values[year];
            if ( vector.isEmpty() )
            {
                vector.resize(12);
                qFill(vector, 0.0);
            }
            
            vector[month - 1] = count;
        }

        if (values.isEmpty())
        {
            const int year = QDate::currentDate().year();
            const QVector<double> emptyVector(12, 0.0);
            values[year] = emptyVector;
        }
        
        foreach (int year, values.keys())
        {
            setDataset(curveNumber, values[year], QString::number(year));
            QPen pen(lineDiagram()->pen());
            const QColor color = curvesColor[colorIndex++];
            pen.setWidth(2);
            pen.setColor(color);
            lineDiagram()->setPen(curveNumber, pen);
            lineDiagram()->setBrush(curveNumber, color);

            DataValueAttributes dva(lineDiagram()->dataValueAttributes(curveNumber));
            MarkerAttributes markers(dva.markerAttributes());
            markers.setMarkerStyle(MarkerAttributes::MarkerDiamond);
            markers.setMarkerSize(QSizeF(7, 7));
            markers.setVisible(true);
            dva.setMarkerAttributes(markers);
            dva.setVisible(true);
            lineDiagram()->setDataValueAttributes(curveNumber, dva);
            ++curveNumber;


            if (colorIndex == colorsCount)
            {
                colorIndex = 0;
            }
        }
    }
}
