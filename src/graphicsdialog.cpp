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
#include <KDChart/KDChartThreeDBarAttributes>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartThreeDLineAttributes>
#include <KDChart/KDChartLineDiagram>

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

    ui->activityButtonGroup->setId(ui->radActivityLine, KDChart::Widget::Line);
    ui->activityButtonGroup->setId(ui->radActivityColumn, KDChart::Widget::Bar);

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

void GraphicsDialog::on_activityButtonGroup_buttonClicked(int id)
{
    const KDChart::Widget::ChartType type = static_cast<KDChart::Widget::ChartType>(id);
    ui->activityChart->setType(type);

    const bool ThreeD = ui->chk3D->isChecked();
    on_chk3D_clicked(ThreeD);
}

void GraphicsDialog::on_chk3D_clicked(bool checked)
{
    if (ui->activityChart->type() == KDChart::Widget::Bar)
    {
        KDChart::BarDiagram *barDiagram = ui->activityChart->barDiagram();
        Q_ASSERT(barDiagram);
        KDChart::ThreeDBarAttributes td(barDiagram->threeDBarAttributes());
        td.setEnabled(checked);
        barDiagram->setThreeDBarAttributes(td);
        ui->activityChart->update();
    }
    else if (ui->activityChart->type() == KDChart::Widget::Line)
    {
        KDChart::LineDiagram *lineDiagram = ui->activityChart->lineDiagram();
        Q_ASSERT(lineDiagram);
        KDChart::ThreeDLineAttributes td(lineDiagram->threeDLineAttributes());
        td.setEnabled(checked);
        lineDiagram->setThreeDLineAttributes(td);
        ui->activityChart->update();
    }

    ui->activityChart->resetColors();
}
