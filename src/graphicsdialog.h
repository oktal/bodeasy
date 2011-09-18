#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>

class ActivityChart;

class GraphicsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GraphicsDialog(QWidget *parent = 0);

private:
    ActivityChart *mPlot;

};

#endif // GRAPHICSDIALOG_H
