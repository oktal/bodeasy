#include "exercisewidget.h"
#include "ui_exercisewidget.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLineEdit>

ExerciseWidget::ExerciseWidget(qint64 id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget),
    mSessionExerciseId(id)
{
    ui->setupUi(this);
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}

void ExerciseWidget::setExerciseName(const QString &exerciseName)
{
    mExerciseName = exerciseName;
    ui->lblExerciseName->setText(exerciseName);
}
