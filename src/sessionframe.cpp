#include "sessionframe.h"
#include "ui_sessionframe.h"
#include "exercisewidget.h"

SessionFrame::SessionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SessionFrame)
{
    ui->setupUi(this);

    ui->gridLayout->addWidget(new ExerciseWidget, 0, 0);
    ui->gridLayout->addWidget(new ExerciseWidget, 0, 1);
    ui->gridLayout->addWidget(new ExerciseWidget, 1, 0);
    ui->gridLayout->addWidget(new ExerciseWidget, 1, 1);
}

SessionFrame::~SessionFrame()
{
    delete ui;
}

QSize SessionFrame::sizeHint() const
{
    return QSize(600, 500);
}
