#include "legendmodel.h"

#include <KDChart/KDChartLegend>
#include <KDChart/KDChartBackgroundAttributes>
#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartMarkerAttributes>

LegendModel::LegendModel(QObject *parent) :
    QAbstractListModel(parent),
    mLegend(0)
{
}

void LegendModel::setLegend(KDChart::Legend *legend)
{
    beginResetModel();
    mLegend = legend;
    /* By default, all items are checked */
    mCheckedDatasets.clear();
    for (int i = 0; i < mLegend->datasetCount(); ++i)
    {
        mCheckedDatasets.append(i);
    }
    endResetModel();
}

Qt::ItemFlags LegendModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    flags |= Qt::ItemIsUserCheckable;
    return flags;
}

int LegendModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mLegend)
    {
        return 0;
    }

    return mLegend->datasetCount();
}

QVariant LegendModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() < 0 || index.row() >
            static_cast<int>(mLegend->datasetCount()))
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return mLegend->diagram()->datasetLabels().at(index.row());
//    case Qt::ForegroundRole:
//        return mLegend->diagram()->datasetBrushes().at(index.row());
    case Qt::FontRole:
    {
        QFont font(mLegend->font());
        font.setBold(true);
        return font;
    }
    case Qt::DecorationRole:
        return cachePixmap(index);
    case Qt::CheckStateRole:
        return mCheckedDatasets.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool LegendModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >
            static_cast<int>(mLegend->datasetCount()))
    {
        return false;
    }

    if (role == Qt::CheckStateRole)
    {
        const bool checked = value.toBool();
        if (checked)
        {
            mCheckedDatasets.append(index.row());
        }
        else
        {
            mCheckedDatasets.removeOne(index.row());
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QPixmap LegendModel::cachePixmap(const QModelIndex &index) const
{
    const QString key = QString("%1:%2").arg(index.row()).arg(index.column());
    const QSizeF size(16, 16);
    const QPoint center(8, 8);
    QPixmap pixmap(size.toSize());
    if (!mCache.find(key, &pixmap))
    {
        pixmap.fill(Qt::transparent);
        const KDChart::MarkerAttributes atts = mLegend->diagram()->datasetMarkers().at(index.row());
        const QBrush brush = mLegend->diagram()->datasetBrushes().at(index.row());
        const QPen pen = atts.pen();
        QPainter painter(&pixmap);
        mLegend->diagram()->paintMarker(&painter, atts, brush, pen, center, size);
    }

    return pixmap;
}

QVariant LegendModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        Q_ASSERT(section == 0);
        return tr("Legend");
    }

    return QVariant();
}

