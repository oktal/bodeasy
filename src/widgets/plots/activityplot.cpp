#include "activityplot.h"

#include <qwt_plot_histogram.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend_item.h>
#include <qwt_scale_engine.h>
#include <QPen>
#include <QColor>
#include <QVector>
#include <QFont>
#include <QDate>
#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

#include "../../sql/SqlHelper.h"

class MonthScaleDraw  : public QwtScaleDraw
{
public:
    virtual QwtText label(double value) const
    {
        const int m = static_cast<int>(value);
        const QLocale locale;
        if (m >= 0 && m <= 11)
        {
            return locale.monthName(m +1, QLocale::ShortFormat);
        }

        return QwtText(" ");
    }

};


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

/*!
  * \brief Create the curve according to the values with the \a year title
  * \return the created curve
*/
static QwtPlotCurve *makeCurve(const QVector<QPointF>& points, const QString &year)
{
    QwtPlotCurve *curve = new QwtPlotCurve(year);
    curve->setData(new QwtPointSeriesData(points));

    QPen curvePen;
    const QColor curveColor = curvesColor[colorIndex++];
    curvePen.setColor(curveColor);
    curvePen.setWidthF(2.0);
    curve->setPen(curvePen);

    QwtSymbol *symbol = new QwtSymbol;
    symbol->setStyle(QwtSymbol::Ellipse);
    symbol->setSize(6);
    symbol->setColor(curveColor.darker());
    curve->setSymbol(symbol);

    if (colorIndex == colorsCount)
    {
        colorIndex = 0;
    }

    return curve;
}


ActivityPlot::ActivityPlot(QWidget *parent) :
    QwtPlot(parent)
{
    setTitle(QwtText(trUtf8("Activité annuelle")));
    setAxisScale(QwtPlot::xBottom, 0.0, 11.0, 1.0);
    setAxisTitle(QwtPlot::yLeft, QwtText(trUtf8("Séances effectuées")));
    setAxisScaleDraw(QwtPlot::xBottom, new MonthScaleDraw);
    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom);
    setAxisMaxMinor(QwtPlot::xBottom, 0);
    setAxisMaxMinor(QwtPlot::yLeft, 0);
    setAxisAutoScale(QwtPlot::yLeft, true);
    setCanvasLineWidth(1);


    QFont xAxisFont("Arial", 10);
    setAxisFont(QwtPlot::xBottom, xAxisFont);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(false);
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(QColor(0, 51, 72));
    grid->setPen(pen);
    grid->attach(this);

    QwtLegend *legend = new QwtLegend;
    insertLegend(legend, QwtPlot::RightLegend);

    colorIndex = 0;

    retrieveDatas();
    updateAxes(); // force axis update to get correct max value
    setAxisScale(QwtPlot::yLeft, 0.0, axisInterval( QwtPlot::yLeft ).maxValue(), 1.0);
}

void ActivityPlot::retrieveDatas()
{
    const QString sql = "SELECT strftime('%Y', date) AS year, strftime('%m', date) AS month, COUNT(date) AS count FROM session_made GROUP BY strftime('%Y%m', date)";
    QSqlQuery query = SqlHelper::query();
    if (query.exec(sql))
    {
        QMap<int, QVector<QPointF> > values; // year, month/count
        while (query.next())
        {
            const int year = query.value(0).toInt();
            const int month = query.value(1).toInt();
            const int count = query.value(2).toInt();
            QVector<QPointF>& vector = values[year];
            if ( vector.isEmpty() )
            {
                vector.resize(12);
                for (int i = 0; i < vector.count(); i++)
                {
                    vector[i] = QPointF(i, 0);
                }
            }
            
            vector[month -1].setY(count);
        }
        
        foreach (const int& year, values.keys())
        {
            QwtPlotCurve *curve = makeCurve(values.value(year), QString::number(year));
            curve->attach(this);
            mCurves.append(curve);
        }
    }
}
