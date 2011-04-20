#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>

namespace Ui
{
    class ExerciseWidget;
}

class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(QWidget *parent = 0);
    ~ExerciseWidget();

private:
    Ui::ExerciseWidget *ui;

};

#endif // EXERCISEWIDGET_H
