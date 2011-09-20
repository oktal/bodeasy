#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <QAbstractListModel>
#include <QPixmapCache>

namespace KDChart
{
class Legend;
}

class LegendModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LegendModel(QObject *parent = 0);
    void setLegend(KDChart::Legend *legend);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QPixmap cachePixmap(const QModelIndex &index) const;

    KDChart::Legend *mLegend;
    QList<int> mCheckedDatasets;

    mutable QPixmapCache mCache;
};

#endif // LEGENDMODEL_H
