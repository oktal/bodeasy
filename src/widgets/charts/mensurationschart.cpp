#include "mensurationschart.h"

#include <KDChart/KDChartWidget>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartGridAttributes>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartLegend>

#include <QAbstractItemModel>

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

MensurationsChart::MensurationsChart(QWidget *parent) :
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

    // show data values
    DataValueAttributes a(lineDiagram()->dataValueAttributes());
    a.setVisible(true);
    lineDiagram()->setDataValueAttributes(a);


    setGlobalLeading( 20, 20, 20, 20 );

    CartesianCoordinatePlane *plane = static_cast<CartesianCoordinatePlane *>
            (lineDiagram()->coordinatePlane());
    GridAttributes attributes(plane->gridAttributes(Qt::Vertical));
    attributes.setGridVisible(  true );
    attributes.setSubGridVisible( true );
    plane->setGridAttributes(Qt::Vertical, attributes);

    colorIndex = 0;

    const QAbstractItemModel *model = lineDiagram()->model();
    for (int i = 0; i < 10; ++i)
    {
        if (model->data(model->index(0, i)).isValid())
        {
            DataValueAttributes dva(lineDiagram()->dataValueAttributes(i));
            MarkerAttributes markers(dva.markerAttributes());
            markers.setMarkerStyle(MarkerAttributes::MarkerDiamond);
            markers.setMarkerSize(QSizeF(7, 7));
            markers.setVisible(true);
            dva.setMarkerAttributes(markers);
            dva.setVisible(true);
            lineDiagram()->setDataValueAttributes(i, dva);
        }
    }

}

void MensurationsChart::retrieveDatas()
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT id_mensuration, neck, shoulder, chest, waist_size, "
                  "forearm, thigh, calf, biceps, wrist, ankle, size, date "
                  "FROM mensuration "
                  "WHERE date>=:startDate AND date<=:endDate");
    query.bindValue(":startDate", mStartDate);
    query.bindValue(":endDate", mEndDate);
    if (query.exec())
    {
        QVector<double> neckValues;
        QVector<double> shoulderValues;
        QVector<double> chestValues;
        QVector<double> waistSizeValues;
        QVector<double> foreArmValues;
        QVector<double> thighValues;
        QVector<double> calfValues;
        QVector<double> bicepsValues;
        QVector<double> wristValues;
        QVector<double> ankleValues;
        QVector<double> sizeValues;

        QStringList axisLabels;
        QStringList shortAxisLabels;
        while (query.next())
        {
            neckValues << query.value(1).toDouble();
            shoulderValues << query.value(2).toDouble();
            chestValues << query.value(3).toDouble();
            waistSizeValues << query.value(4).toDouble();
            foreArmValues << query.value(5).toDouble();
            thighValues << query.value(6).toDouble();
            calfValues << query.value(7).toDouble();
            bicepsValues << query.value(8).toDouble();
            wristValues << query.value(9).toDouble();
            ankleValues << query.value(10).toDouble();
            sizeValues << query.value(11).toDouble();

            const QDateTime date = query.value(12).toDateTime();
            axisLabels << date.toString(Qt::SystemLocaleLongDate);
            shortAxisLabels << date.toString(Qt::SystemLocaleShortDate);
        }

        setDataset(0, neckValues, tr("Neck"));
        setDataset(1, shoulderValues, tr("Shoulder"));
        setDataset(2, chestValues, tr("Chest"));
        setDataset(3, waistSizeValues, tr("Waist size"));
        setDataset(4, foreArmValues, tr("Forearm"));
        setDataset(5, thighValues, tr("Thigh"));
        setDataset(6, calfValues, tr("Calf"));
        setDataset(7, bicepsValues, tr("Biceps"));
        setDataset(8, wristValues, tr("Wrist"));
        setDataset(9, ankleValues, tr("Ankle"));
        //setDataset(10, sizeValues, tr("Size"));

        xAxis->setLabels(axisLabels);
        xAxis->setShortLabels(shortAxisLabels);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << query.lastError();
    }
}

void MensurationsChart::setStartDate(const QDate &date)
{
    Q_ASSERT(date <= mEndDate);
    retrieveDatas();
}

void MensurationsChart::setEndDate(const QDate &date)
{
    Q_ASSERT(date >= mStartDate);
    retrieveDatas();
}
