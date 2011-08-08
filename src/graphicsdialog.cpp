#include "graphicsdialog.h"
#include "widgets/plots/activityplot.h"

#include <QDate>
#include <QVBoxLayout>


GraphicsDialog::GraphicsDialog(QWidget *parent) :
    QDialog(parent)
{
    mPlot = new ActivityPlot(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mPlot);
    setLayout(layout);

    mPlot->replot();
}
