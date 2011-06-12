#include "sessionframe.h"
#include "ui_sessionframe.h"
#include "exercisewidget.h"
#include "sql/SqlHelper.h"
#include "sql/exercise.h"
#include "exercisewidgetdata.h"
#include "SessionProxy.h"
#include "widgets/sessioncontrolwidget.h"

#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

static int const RowMaximumWidgets = 2;
static int const ColumnMaximumWidgets = 2;

SessionFrame::SessionFrame(SessionProxy *proxy) :
    QFrame(proxy),
    ui(new Ui::SessionFrame),
    mCurrentPage(0),
    mProxy(proxy)
{
    ui->setupUi(this);
    ui->lblObjective->hide();
    ui->chkObjectiveDone->hide();
    connect(ui->controlWidget, SIGNAL(finishClicked()), mProxy, SLOT(stop()));

    ui->controlWidget->setEnabled(false);
    connect(ui->controlWidget, SIGNAL(firstClicked()), this, SLOT(btnFirstClicked()));
    connect(ui->controlWidget, SIGNAL(lastClicked()), this, SLOT(btnLastClicked()));
    connect(ui->controlWidget, SIGNAL(previousClicked()), this, SLOT(btnPreviousClicked()));
    connect(ui->controlWidget, SIGNAL(nextClicked()), this, SLOT(btnNextClicked()));
}

SessionFrame::~SessionFrame()
{
    delete ui;
}

QSize SessionFrame::sizeHint() const
{
    return QSize(600, 500);
}

void SessionFrame::setWidgetsData(const ExerciseWidgetDataList &data, bool readOnly)
{
    mCurrentPage = 0;

    /* Remove all existing pages */

    QWidget *w;
    while ((w = ui->stackedWidget->currentWidget()) != 0)
    {
        delete w;
    }

    exercises.clear();

    foreach (const ExerciseWidgetData &d, data)
    {
        ExerciseWidget *ew = new ExerciseWidget;
        ew->setData(d);
        ew->setReadOnly(readOnly);
        exercises.append(ew);
    }

    paginate();
    ui->stackedWidget->setCurrentIndex(0);

    start();
    if (readOnly)
    {
      //  ui->btnFinish->setEnabled(false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::FinishButton, false);
    }
}

void SessionFrame::setObjective(const QString &objective)
{
    ui->lblObjective->show();
    ui->chkObjectiveDone->show();
    ui->lblObjective->setText(objective);
}


ExerciseWidgetDataList SessionFrame::widgetsData() const
{
    ExerciseWidgetDataList list;
    foreach (ExerciseWidget *ew, exercises)
    {
        list << ew->data();
    }

    return list;
}

void SessionFrame::start()
{
    ui->controlWidget->setEnabled(true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::FirstButton, false);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::PreviousButton, false);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::FinishButton, true);
    // ui->btnFirst->setEnabled(false);
   // ui->btnPrevious->setEnabled(false);
   // ui->btnFinish->setEnabled(true);
    /* Enable Last and Next buttons if there are more than one page */
    if (ui->stackedWidget->count() == 1)
    {
     //   ui->btnLast->setEnabled(false);
       // ui->btnNext->setEnabled(false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::LastButton, false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::NextButton, false);
    }
}


void SessionFrame::btnNextClicked()
{
    ++mCurrentPage;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
    if (mCurrentPage == ui->stackedWidget->count() - 1)
    {
      //  ui->btnLast->setEnabled(false);
       // ui->btnNext->setEnabled(false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::LastButton, false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::NextButton, false);
    }

   // ui->btnFirst->setEnabled(true);
   // ui->btnPrevious->setEnabled(true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::FirstButton, true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::PreviousButton, true);
}

void SessionFrame::btnPreviousClicked()
{
    --mCurrentPage;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
    if (mCurrentPage == 0)
    {
       // ui->btnFirst->setEnabled(false);
       // ui->btnPrevious->setEnabled(false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::FirstButton, false);
        ui->controlWidget->setButtonEnabled(SessionControlWidget::PreviousButton, false);
    }

    // ui->btnLast->setEnabled(true);
    // ui->btnNext->setEnabled(true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::LastButton, true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::NextButton, true);
}

void SessionFrame::btnFirstClicked()
{
    mCurrentPage = 0;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);
  //  ui->btnFirst->setEnabled(false);
  //  ui->btnPrevious->setEnabled(false);

    ui->controlWidget->setButtonEnabled(SessionControlWidget::FirstButton, false);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::PreviousButton, false);


   // ui->btnLast->setEnabled(true);
   // ui->btnNext->setEnabled(true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::LastButton, true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::NextButton, true);
}

void SessionFrame::btnLastClicked()
{
    mCurrentPage = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(mCurrentPage);

    //ui->btnNext->setEnabled(false);
    //ui->btnLast->setEnabled(false);

    ui->controlWidget->setButtonEnabled(SessionControlWidget::LastButton, false);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::NextButton, false);

   // ui->btnPrevious->setEnabled(true);
   // ui->btnFirst->setEnabled(true);

    ui->controlWidget->setButtonEnabled(SessionControlWidget::FirstButton, true);
    ui->controlWidget->setButtonEnabled(SessionControlWidget::PreviousButton, true);
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
