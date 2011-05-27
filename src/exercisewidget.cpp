#include "exercisewidget.h"
#include "ui_exercisewidget.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLineEdit>


ExerciseWidget::ExerciseWidget(qint64 id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget),
    mSessionExerciseId(id),
    mUseWeight(true)
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

void ExerciseWidget::setExerciseType(Exercise::Type type)
{
    switch (type)
    {
    case Exercise::Repetition:
        ui->lblType->setText(trUtf8("Répétition"));
        break;
    case Exercise::Duration:
        ui->lblType->setText(trUtf8("Durée"));
        break;
    }
}

void ExerciseWidget::setExerciseDifficulty(Exercise::Difficulty difficulty)
{
    switch (difficulty)
    {
    case Exercise::Easy:
        ui->lblDifficulty->setText(trUtf8("Facile"));
        break;
    case Exercise::Medium:
        ui->lblDifficulty->setText(trUtf8("Moyen"));
        break;
    case Exercise::Hard:
        ui->lblDifficulty->setText(trUtf8("Difficile"));
        break;
    }
}

void ExerciseWidget::setExerciseRest(int rest)
{
    ui->lblRest->setText(QString("%1 secs.").arg(rest));
}

void ExerciseWidget::setExerciseUseWeight(bool weight)
{
    mUseWeight = weight;
}

void ExerciseWidget::setExerciseRepetitions(int repetitions)
{
    ui->lblRepetitions->setNum(repetitions);
}

void ExerciseWidget::setExerciseSeries(int series)
{
    delete ui->scrollAreaWidgetContents->layout();
    mPairs.clear();

    /* Rebuild the layout */

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setHorizontalSpacing(20);
    layout->addWidget(new QLabel(trUtf8("Rés.")), 0, 1);
    layout->addWidget(new QLabel(trUtf8("Charge")), 0, 2);
    int row = 1;
    for (int i = 0; i < series; ++i)
    {
        layout->addWidget(new QLabel(trUtf8("Série %1").arg(i + 1)), row, 0);


        QLineEdit *txtSerieResult = new QLineEdit;
        txtSerieResult->setFixedWidth(30);
        layout->addWidget(txtSerieResult, row, 1);

        QLineEdit *txtLoad = new QLineEdit;
        txtLoad->setFixedWidth(30);
        layout->addWidget(txtLoad, row, 2);
        ++row;

        mPairs.append(qMakePair(txtSerieResult, txtLoad));
    }

    ui->scrollAreaWidgetContents->setLayout(layout);
}

void ExerciseWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange)
    {
        /* Enable or disable all loads lineEdits */
        QListIterator<QPair<QLineEdit *, QLineEdit *> > it(mPairs);
        while (it.hasNext())
        {
            it.next().second->setEnabled(mUseWeight);
        }
    }
}
