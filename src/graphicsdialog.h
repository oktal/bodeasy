#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>

class ActivityPlot;

class GraphicsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GraphicsDialog(QWidget *parent = 0);

private:
    ActivityPlot *mPlot;

};

#endif // GRAPHICSDIALOG_H
