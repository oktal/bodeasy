#ifndef PLANNINGWINDOW_H
#define PLANNINGWINDOW_H

#include <QMainWindow>

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

private:
    Ui::PlanningWindow *ui;
    BasicCalendarModel *mCalendarModel;
};

#endif // PLANNINGWINDOW_H
