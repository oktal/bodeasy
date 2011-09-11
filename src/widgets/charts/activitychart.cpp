#include "activitychart.h"

#include <KDChart/KDChartWidget>
#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartMarkerAttributes>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartBackgroundAttributes>
#include <KDChart/KDChartGridAttributes>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartThreeDLineAttributes>
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
static const QColor colors[] = {
    QColor(65, 111, 166),
    QColor(168, 66, 63),
    QColor(134, 164, 74),
    QColor(110, 84, 141),
    QColor(61, 150, 174),
    QColor(218, 129, 55),
    QColor(142, 165, 203),
    QColor(206, 142, 141),
    QColor(181, 202, 146),
    QColor(165, 151, 185)
};

const size_t colorsCount = sizeof colors / sizeof *colors;

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

    resetColors();

}

void ActivityChart::retrieveDatas()
{
    const QString sql = "SELECT strftime('%Y', date) AS year, strftime('%m', date) AS month, COUNT(date) AS count FROM session_made GROUP BY strftime('%Y%m', date)";
    QSqlQuery query = SqlHelper::query();
    int curveNumber = 0;
    if (query.exec(sql))
    {
        mValues.clear();
        while (query.next())
        {
            const int year = query.value(0).toInt();
            const int month = query.value(1).toInt();
            const int count = query.value(2).toInt();
            QVector<double>& vector = mValues[year];
            if ( vector.isEmpty() )
            {
                vector.resize(12);
                qFill(vector, 0.0);
            }
            
            vector[month - 1] = count;
        }

        if (mValues.isEmpty())
        {
            const int year = QDate::currentDate().year();
            const QVector<double> emptyVector(12, 0.0);
            mValues[year] = emptyVector;
        }

        foreach (int year, mValues.keys())
        {
            setDataset(curveNumber++, mValues[year], QString::number(year));
        }
        
    }
}

void ActivityChart::resetColors()
{
    colorIndex = 0;

    for (int i = 0; i < mValues.size(); ++i)
    {
        const QColor color = colors[colorIndex++];
        diagram()->setBrush(i, color);

        QPen pen(diagram()->pen(i));
        if (type() == Widget::Bar)
        {
            pen.setWidthF(1);
            pen.setColor(Qt::black);
        }
        else if (type() == Widget::Line)
        {
            ThreeDLineAttributes tda(lineDiagram()->threeDLineAttributes());
            pen.setWidthF(tda.isEnabled() ? 0.5 : 2.0);
            pen.setColor(tda.isEnabled() ? Qt::black : color);
            if (tda.isEnabled())
            {
                QColor c(color);
                c.setAlpha(200);
                diagram()->setBrush(i, QBrush(c));
            }
        }

        diagram()->setPen(i, pen);

        DataValueAttributes dva(diagram()->dataValueAttributes(i));
        MarkerAttributes markers(dva.markerAttributes());
        markers.setMarkerStyle(MarkerAttributes::MarkerDiamond);
        markers.setMarkerSize(QSizeF(7, 7));
        markers.setVisible(true);
        dva.setMarkerAttributes(markers);
        dva.setVisible(true);
        diagram()->setDataValueAttributes(i, dva);


        if (colorIndex == colorsCount)
        {
            colorIndex = 0;
        }
    }
}
