#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>

class ActivityChart;
class WeightChart;
class QTreeView;
class QModelIndex;
class LegendModel;

namespace Ui
{
class GraphicsDialog;
}

class GraphicsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GraphicsDialog(QWidget *parent = 0);
    ~GraphicsDialog();

private slots:
    void onActivityModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void onMensuratonsModelChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    Ui::GraphicsDialog *ui;
    LegendModel *mActivityModel;
    LegendModel *mMensurationsModel;

};

#endif // GRAPHICSDIALOG_H
