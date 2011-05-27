#include "sessionframe.h"
#include "ui_sessionframe.h"
#include "exercisewidget.h"
#include "sql/SqlHelper.h"

#include <QDebug>
#include <QWidget>

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
    mCurrentPage = 0;

    for (int i = 0; i < ui->stackedWidget->count(); ++i)
    {
        ui->stackedWidget->removeWidget(ui->stackedWidget->widget(i));
    }

    qDeleteAll(exercises);
    exercises.clear();

    selectExercises();
    paginate();
    ui->stackedWidget->setCurrentIndex(0);
}

void SessionFrame::start()
{
    setEnabled(true);
    ui->btnFirst->setEnabled(false);
    ui->btnPrevious->setEnabled(false);
    if (ui->stackedWidget->count() == 1)
    {
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
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

void SessionFrame::selectExercises()
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT se.id_exercise, e.name FROM "
                  "session_exercise se INNER JOIN exercise e "
                  "ON se.id_exercise = e.id_exercise "
                  "WHERE id_session=:sessionId");
    query.bindValue(":sessionId", mSessionId);
    if (query.exec())
    {
        while (query.next())
        {
            ExerciseWidget *ew = new ExerciseWidget(query.value(0).toLongLong());
            ew->setExerciseName(query.value(1).toString());
            exercises.append(ew);
        }
    }
}

void SessionFrame::paginate()
{
    int row = 0, column = 0;
    int page = 0;
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(QMargins(0, 0, 0, 0));
    foreach (ExerciseWidget *ew, exercises)
    {
        layout->addWidget(ew, row, column++);
        if (column == 2)
        {
            ++row;
            if (row == 2)
            {
                row = 0;
                QWidget * widget = new QWidget;
                widget->setLayout(layout);
                ui->stackedWidget->addWidget(widget);
                ++page;
                layout = new QGridLayout;
                layout->setContentsMargins(QMargins(0, 0, 0, 0));
            }
            column = 0;
        }
    }

    if (row != 2 && column != 2)
    {

        for (int r = row; r < 2; ++r)
        {
            for (int c = column; c < 2; ++c)
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
