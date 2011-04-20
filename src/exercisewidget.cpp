#include "exercisewidget.h"
#include "ui_exercisewidget.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLineEdit>

ExerciseWidget::ExerciseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget)
{
    ui->setupUi(this);
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}
