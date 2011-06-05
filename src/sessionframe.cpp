#include "sessionframe.h"
#include "ui_sessionframe.h"
#include "exercisewidget.h"
#include "sql/SqlHelper.h"
#include "sql/exercise.h"
#include "exercisewidgetdata.h"

#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

static int const RowMaximumWidgets = 2;
static int const ColumnMaximumWidgets = 2;

SessionFrame::SessionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SessionFrame),
    mCurrentPage(0)
{
    ui->setupUi(this);
}

SessionFrame::~SessionFrame()
{
    delete ui;
}

QSize SessionFrame::sizeHint() const
{
    return QSize(600, 500);
}

void SessionFrame::setSessionId(qint64 id)
{
    mSessionId = id;
    refresh();
}

void SessionFrame::setUserId(qint64 id)
{
    mUserId = id;
}

void SessionFrame::refresh()
{
    mCurrentPage = 0;

    /* Remove all existing pages */

    QWidget *w;
    while ((w = ui->stackedWidget->currentWidget()) != 0)
    {
        delete w;
    }

    exercises.clear();

    selectExercises();
    paginate();
    ui->stackedWidget->setCurrentIndex(0);
}

void SessionFrame::start()
{
    ui->btnFirst->setEnabled(false);
    ui->btnPrevious->setEnabled(false);
    ui->btnFinish->setEnabled(true);
    /* Enable Last and Next buttons if there are more than one page */
    if (ui->stackedWidget->count() == 1)
    {
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
    }
}

void SessionFrame::stop()
{
    QList<ExerciseWidget *>::const_iterator it;

    bool isComplete = true;

    /* Check if each exercise is complete */
    for (it = exercises.constBegin(); it != exercises.constEnd(); ++it)
    {
        ExerciseWidget *ew = *it;
        if (!ew->isComplete())
        {
            isComplete = false;
            break;
        }
    }


    if (!isComplete)
    {
        const int r = QMessageBox::warning(this,
                                            trUtf8("Attention"),
                                            trUtf8("Certains champs n'ont pas été remplis. Ils seront "
                                                   "mis à 0. Voulez-vous vraiment terminer la séance ?"),
                                            QMessageBox::Yes, QMessageBox::No);
        if (r == QMessageBox::No)
        {
            return;
        }
    }

    /* Persist */
    const bool transaction = SqlHelper::transaction();
    bool ok = true;

    qint64 sessionMadeId = -1;
    {
        QSqlQuery query = SqlHelper::query();
        query.prepare("INSERT INTO session_made(id_session, id_user, date) "
                      "VALUES(:id_session, :id_user, :date)");
        query.bindValue(":id_session", mSessionId);
        query.bindValue(":id_user", mUserId);
        query.bindValue(":date", QDateTime::currentDateTime());
        if (!query.exec())
        {
            if (transaction)
            {
                SqlHelper::rollback();
                ok = false;
            }
        }
        else
        {
            sessionMadeId = query.lastInsertId().toLongLong();
        }
    }

    if (ok)
    {
        for (it = exercises.constBegin(); it != exercises.constEnd(); ++it)
        {
            ExerciseWidget *ew = *it;
            if(!ew->save(mUserId, mSessionId, sessionMadeId))
            {
                if (transaction)
                {
                    SqlHelper::rollback();
                }
                ok = false;
                break;
            }
        }
    }

    if (transaction && ok)
    {
        SqlHelper::commit();
    }

    ui->btnFinish->setEnabled(false);
    if (ok)
    {
        QMessageBox::information(this, trUtf8("Information"),
                                 trUtf8("La séance du %1 a bien été enregistrée.")
                                 .arg(QDate::currentDate().toString(Qt::SystemLocaleLongDate)));
        emit sessionFinished();
    }
    else
    {
        QMessageBox::critical(this, trUtf8("Erreur"),
                              trUtf8("Erreur lors de l'enregistrement de la séance: %1")
                              .arg(SqlHelper::lastError()));
    }
}

void SessionFrame::on_btnNext_clicked()
{
    ++mCurrentPage;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
    if (mCurrentPage == ui->stackedWidget->count() - 1)
    {
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
    }

    ui->btnFirst->setEnabled(true);
    ui->btnPrevious->setEnabled(true);
}

void SessionFrame::on_btnPrevious_clicked()
{
    --mCurrentPage;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
    if (mCurrentPage == 0)
    {
        ui->btnFirst->setEnabled(false);
        ui->btnPrevious->setEnabled(false);
    }

    ui->btnLast->setEnabled(true);
    ui->btnNext->setEnabled(true);
}

void SessionFrame::on_btnFirst_clicked()
{
    mCurrentPage = 0;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
    ui->btnFirst->setEnabled(false);
    ui->btnPrevious->setEnabled(false);

    ui->btnLast->setEnabled(true);
    ui->btnNext->setEnabled(true);
}

void SessionFrame::on_btnLast_clicked()
{
    mCurrentPage = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);

    ui->btnNext->setEnabled(false);
    ui->btnLast->setEnabled(false);

    ui->btnPrevious->setEnabled(true);
    ui->btnFirst->setEnabled(true);
}

void SessionFrame::on_btnFinish_clicked()
{
    stop();
}

/*!
  * \brief Select all exercises corresponding to the session id
           and create the ExerciseWidget widgets
*/
void SessionFrame::selectExercises()
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT se.id_exercise, e.name, e.type, e.difficulty, e.weight, e.description, "
                  "se.id_session_exercise, se.rest, se.repetitions, se.series FROM "
                  "session_exercise se INNER JOIN exercise e "
                  "ON se.id_exercise = e.id_exercise "
                  "WHERE id_session=:sessionId");
    query.bindValue(":sessionId", mSessionId);
    if (query.exec())
    {
        int number = 1;
        while (query.next())
        {
            ExerciseWidget *ew = new ExerciseWidget();
            ExerciseWidgetData data;
            data.exerciseId = query.value(0).toLongLong();
            data.name = query.value(1).toString();
            data.type = Exercise::Type(query.value(2).toInt());
            data.difficulty = Exercise::Difficulty(query.value(3).toInt());
            data.weight = query.value(4).toBool();
            data.description = query.value(5).toString();
            data.sessionExerciseId = query.value(6).toLongLong();
            data.rest = query.value(7).toInt();
            data.repetitions = query.value(8).toInt();
            data.series = query.value(9).toInt();
            data.seriesData.reserve(data.series);
            //data.number = number;
            ew->setData(data);
            exercises.append(ew);
            ++number;
        }
    }
}

/*!
  * \brief Paginate the exercises
*/
void SessionFrame::paginate()
{
    if (exercises.count() == 0)
    {
        return;
    }

    int row = 0, column = 0;
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    foreach (ExerciseWidget *ew, exercises)
    {
        layout->addWidget(ew, row, column++);
        if (column == ColumnMaximumWidgets)
        {
            ++row;
            if (row == RowMaximumWidgets)
            {
                row = 0;
                QWidget * widget = new QWidget;
                widget->setLayout(layout);
                ui->stackedWidget->addWidget(widget);
                layout = new QGridLayout;
                layout->setContentsMargins(QMargins(0, 0, 0, 0));
            }
            column = 0;
        }
    }

    /* Put spacers if the page is not full */
    if (row != RowMaximumWidgets && column != ColumnMaximumWidgets)
    {

        for (int r = row; r < RowMaximumWidgets; ++r)
        {
            for (int c = column; c < ColumnMaximumWidgets; ++c)
            {
                layout->addItem(new QSpacerItem(20, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),
                                r, c);
            }
        }

        QWidget *widget = new QWidget;
        widget->setLayout(layout);
        ui->stackedWidget->addWidget(widget);
    }
}


void SessionFrame::showResults()
{
    start();

    foreach (ExerciseWidget *ew, exercises)
    {
        ew->selectResults();
    }

    ui->btnFinish->setEnabled(false);
}
