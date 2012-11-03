#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class PlanningWindow;
}

class BasicCalendarModel;

class PlanningWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlanningWindow(QWidget *parent = 0);
    ~PlanningWindow();
    
private slots:
    void on_planifyAction_triggered();
    void onItemClicked(const QModelIndex &index);

private:
    Ui::PlanningWindow *ui;
    BasicCalendarModel *mCalendarModel;
};

#endif // PLANNINGWINDOW_H
