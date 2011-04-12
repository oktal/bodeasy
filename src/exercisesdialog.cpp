#include "exercisesdialog.h"
#include "ui_exercisesdialog.h"

#include <QWidget>
#include <QDialog>

ExercisesDialog::ExercisesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExercisesDialog)
{
    ui->setupUi(this);
}

ExercisesDialog::~ExercisesDialog()
{
    delete ui;
}
