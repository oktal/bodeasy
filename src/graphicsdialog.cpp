#include "graphicsdialog.h"
#include "widgets/charts/activitychart.h"
#include "widgets/charts/weightchart.h"
#include "legendmodel.h"

#include <QDate>
#include <QHBoxLayout>
#include <QTreeView>

#include <QDebug>

#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartLegend>

GraphicsDialog::GraphicsDialog(QWidget *parent) :
    QDialog(parent),
    mModel(new LegendModel(this))
{
    mPlot = new WeightChart;
    mTree = new QTreeView;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mTree);
    layout->addWidget(mPlot);
    setLayout(layout);

    mModel->setLegend(mPlot->legend());
    mTree->setModel(mModel);
    mTree->setMinimumWidth(150);

    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(onModelChanged(QModelIndex,QModelIndex)));

}

void GraphicsDialog::onModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_ASSERT(topLeft == bottomRight);

    const int checkValue = topLeft.data(Qt::CheckStateRole).toInt();

    const Qt::CheckState checkState = static_cast<Qt::CheckState>(checkValue);
    mPlot->diagram()->setHidden(topLeft.row(), checkState == Qt::Unchecked);
}

