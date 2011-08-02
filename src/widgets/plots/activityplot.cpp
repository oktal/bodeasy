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
        if (m >= 0 && m <= 11)
        {
            return QDate(2000, m + 1, 1).toString("MMM");
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
static QwtPlotCurve *makeCurve(const double *values, const QString &year)
{
    QwtPlotCurve *curve = new QwtPlotCurve(year);
    QVector<QPointF> points;
    for (int i = 0; i < 12; ++i)
    {
        points << QPointF(static_cast<double>(i), values[i]);
    }
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
    setAxisTitle(QwtPlot::yLeft, QwtText(trUtf8("Séances affectuées")));
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

}

void ActivityPlot::retrieveDatas()
{
    QSqlQuery query = SqlHelper::query();
    if (query.exec("SELECT strftime('%Y', date) FROM session_made "
                   "GROUP BY strftime('%Y', date)"))
    {
        while (query.next())
        {
            const QString year = query.value(0).toString();
            double values[12] = { 0.0 };
            QSqlQuery datasQuery = SqlHelper::query();
            datasQuery.prepare("SELECT date FROM session_made "
                               "WHERE strftime('%Y', date)=:year");
            datasQuery.bindValue(":year", year);
            if (datasQuery.exec())
            {
                while (datasQuery.next())
                {
                    const int month = datasQuery.value(0).toDateTime().date().month();
                    values[month]++;
                }
            }
            else
            {
                qDebug() << Q_FUNC_INFO << datasQuery.lastError();
            }
            QwtPlotCurve *curve = makeCurve(values, year);
            curve->attach(this);

            mCurves.append(curve);
        }
    }
}
