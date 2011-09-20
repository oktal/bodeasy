#include "graphicsdialog.h"
#include "widgets/charts/activitychart.h"
#include "widgets/charts/weightchart.h"
#include "legendmodel.h"

#include "ui_graphicsdialog.h"

#include <QDate>
#include <QHBoxLayout>
#include <QTreeView>

#include <QDebug>

#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartLegend>

GraphicsDialog::GraphicsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicsDialog),
    mModel(new LegendModel(this))
{
    ui->setupUi(this);

    mModel->setLegend(ui->activityChart->legend());
    ui->legendTree->setModel(mModel);

    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(onModelChanged(QModelIndex,QModelIndex)));
}

GraphicsDialog::~GraphicsDialog()
{
    delete ui;
}

void GraphicsDialog::onModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_ASSERT(topLeft == bottomRight);

    const int checkValue = topLeft.data(Qt::CheckStateRole).toInt();

    const Qt::CheckState checkState = static_cast<Qt::CheckState>(checkValue);
    ui->activityChart->diagram()->setHidden(topLeft.row(), checkState == Qt::Unchecked);
}

