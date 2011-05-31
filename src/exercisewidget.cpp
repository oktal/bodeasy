#include "exercisewidget.h"
#include "ui_exercisewidget.h"
#include "sql/SqlHelper.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>
#include <QSqlError>

#include <QDate>

ExerciseWidget::ExerciseWidget(qint64 id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget),
    scrollAreaLayout(0),
    mExerciseId(id),
    mUseWeight(true),
    mExtraSeries(0)
{
    ui->setupUi(this);
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}

void ExerciseWidget::on_btnAddSerie_clicked()
{
    ++mExtraSeries;
    QLineEdit *txtResult = new QLineEdit;
    txtResult->setFixedWidth(30);
    QLineEdit *txtLoad = new QLineEdit;
    txtLoad->setFixedWidth(30);
    txtLoad->setEnabled(mUseWeight);

    mPairs.append(qMakePair(txtResult, txtLoad));
    const int rowCount = mPairs.count();
    scrollAreaLayout->addWidget(new QLabel(trUtf8("Série %1").arg(rowCount)), rowCount, 0);
    scrollAreaLayout->addWidget(txtResult, rowCount, 1);
    scrollAreaLayout->addWidget(txtLoad, rowCount, 2);

    ui->btnDeleteSerie->setEnabled(true);

}

void ExerciseWidget::on_btnDeleteSerie_clicked()
{
    const int rowCount = mPairs.count();
    for (int c = 0; c < scrollAreaLayout->columnCount(); ++c)
    {
        delete scrollAreaLayout->itemAtPosition(rowCount, c)->widget();
    }

    mPairs.removeLast();
    --mExtraSeries;
    if (mExtraSeries == 0)
    {
        ui->btnDeleteSerie->setEnabled(false);
    }
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
    delete scrollAreaLayout;
    mPairs.clear();

    /* Rebuild the layout */

    scrollAreaLayout = new QGridLayout;
    scrollAreaLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);
    scrollAreaLayout->setHorizontalSpacing(20);
    scrollAreaLayout->addWidget(new QLabel(trUtf8("Rés.")), 0, 1);
    scrollAreaLayout->addWidget(new QLabel(trUtf8("Charge")), 0, 2);
    int row = 1;
    for (int i = 0; i < series; ++i)
    {
        scrollAreaLayout->addWidget(new QLabel(trUtf8("Série %1").arg(i + 1)), row, 0);


        QLineEdit *txtSerieResult = new QLineEdit;
        txtSerieResult->setFixedWidth(30);
        scrollAreaLayout->addWidget(txtSerieResult, row, 1);

        QLineEdit *txtLoad = new QLineEdit;
        txtLoad->setFixedWidth(30);
        scrollAreaLayout->addWidget(txtLoad, row, 2);
        ++row;

        mPairs.append(qMakePair(txtSerieResult, txtLoad));
    }

    ui->scrollAreaWidgetContents->setLayout(scrollAreaLayout);
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

        ui->btnDeleteSerie->setEnabled(false);
    }
}

bool ExerciseWidget::isComplete() const
{
     QListIterator<QPair<QLineEdit *, QLineEdit *> > it(mPairs);
     while (it.hasNext())
     {
         QPair<QLineEdit *, QLineEdit *> pair = it.next();
         if (pair.first->text().isEmpty() || pair.second->text().isEmpty())
         {
             return false;
         }
     }

     return true;
}

/**
  * \brief persist the exercise in database denoted by a \a userId and \a sessionId
*/

bool ExerciseWidget::save(qint64 userId, qint64 sessionId)
{
    QSqlQuery q = SqlHelper::query();
    /* TO FIX : id_exercice => id_exercise */
    q.prepare("INSERT INTO exercise_result(result, load, date, serie_number, id_exercice, id_session, id_user) "
              "VALUES(:result, :load, :date, :serie_number, :id_exercise, :id_session, :id_user)");
    q.bindValue(":date", QDate::currentDate());
    q.bindValue(":id_session", sessionId);
    q.bindValue(":id_user", userId);
    q.bindValue(":id_exercise", mExerciseId);

    QList<QPair<QLineEdit *, QLineEdit *> >::const_iterator it;
    int serie = 0;
    for (it = mPairs.begin(); it != mPairs.end(); ++it)
    {

        QPair<QLineEdit *, QLineEdit *> pair = *it;

        QLineEdit *result = pair.first;
        QLineEdit *load = pair.second;

        Q_ASSERT(result);
        Q_ASSERT(load);

        q.bindValue(":serie_number", serie++);

        if (result->text().isEmpty())
        {
            result->setText("0");
        }

        q.bindValue(":result", result->text().toInt());

        if (!load->isEnabled())
        {
            q.bindValue(":load", "NULL");
        }
        else
        {
            if (load->text().isEmpty())
            {
                load->setText("0");
            }

            q.bindValue(":load", load->text().toInt());
        }

        if (!q.exec())
        {
            qDebug() << Q_FUNC_INFO << " SQL Error: " << q.lastError();
            return false;
        }
    }

    for (it = mPairs.begin(); it != mPairs.end(); ++it)
    {
        QPair<QLineEdit *, QLineEdit *> pair = *it;
        pair.first->setReadOnly(true);
        pair.second->setReadOnly(true);
    }

    return true;
}
