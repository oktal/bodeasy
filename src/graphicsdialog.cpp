#include "graphicsdialog.h"
#include "widgets/charts/activitychart.h"

#include <QDate>
#include <QVBoxLayout>


GraphicsDialog::GraphicsDialog(QWidget *parent) :
    QDialog(parent)
{
    mPlot = new ActivityChart;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mPlot);
    setLayout(layout);

//    mPlot->replot();
}
