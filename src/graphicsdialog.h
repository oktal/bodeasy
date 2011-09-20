#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>

class ActivityChart;
class QTreeView;
class QModelIndex;
class LegendModel;

class GraphicsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GraphicsDialog(QWidget *parent = 0);

private slots:
    void onModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    LegendModel *mModel;
    QTreeView *mTree;
    ActivityChart *mPlot;

};

#endif // GRAPHICSDIALOG_H
