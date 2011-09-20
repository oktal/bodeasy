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
    mActivityModel(new LegendModel(this)),
    mMensurationsModel(new LegendModel(this))
{
    ui->setupUi(this);

    mActivityModel->setLegend(ui->activityChart->legend());
    ui->legendActivityTree->setModel(mActivityModel);

    mMensurationsModel->setLegend(ui->mensurationsChart->legend());
    ui->legendMensurationsTree->setModel(mMensurationsModel);

    connect(mActivityModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(onActivityModelChanged(QModelIndex,QModelIndex)));

    connect(mMensurationsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(onMensuratonsModelChanged(QModelIndex,QModelIndex)));
}

GraphicsDialog::~GraphicsDialog()
{
    delete ui;
}

void GraphicsDialog::onActivityModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_ASSERT(topLeft == bottomRight);

    const int checkValue = topLeft.data(Qt::CheckStateRole).toInt();

    const Qt::CheckState checkState = static_cast<Qt::CheckState>(checkValue);
    ui->activityChart->diagram()->setHidden(topLeft.row(), checkState == Qt::Unchecked);
}

void GraphicsDialog::onMensuratonsModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_ASSERT(topLeft == bottomRight);

    const int checkValue = topLeft.data(Qt::CheckStateRole).toInt();

    const Qt::CheckState checkState = static_cast<Qt::CheckState>(checkValue);
    ui->mensurationsChart->diagram()->setHidden(topLeft.row(), checkState == Qt::Unchecked);
}

