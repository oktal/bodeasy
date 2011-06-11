#include "exercisewidget.h"
#include "ui_exercisewidget.h"
#include "sql/SqlHelper.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>

#include <QDebug>
#include <QSqlError>

#include <QDate>

#include <limits>

ExerciseWidget::ExerciseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget),
    scrollAreaLayout(0),
    mReadOnly(false)
{
    ui->setupUi(this);
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}

void ExerciseWidget::setReadOnly(bool readOnly)
{
    foreach (const ExerciseWidget::PairSpinBox &pair, mPairs)
    {
        pair.first->setReadOnly(readOnly);
        pair.second->setReadOnly(readOnly);
    }

    ui->btnAddSerie->setEnabled(!readOnly);

    mReadOnly = readOnly;
}

bool ExerciseWidget::isReadOnly() const
{
    return mReadOnly;
}

void ExerciseWidget::on_btnAddSerie_clicked()
{
    addRow();
    mData.modified = true;
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
    mData.seriesData.removeLast();
    mData.modified = true;
    
    ui->btnDeleteSerie->setEnabled(mPairs.count() > mData.series);
}

void ExerciseWidget::spinBox_valueChanged(int value)
{
    QSpinBox* sb = qobject_cast<QSpinBox*>(sender());
    const int row = sb->objectName().section('_', -1, -1).toInt();
    int* variable = 0;
    
    if (sb->objectName().startsWith("txtResult") )
    {
        variable = &mData.seriesData[ row ].first;
    }
    else if (sb->objectName().startsWith("txtLoad") )
    {
        variable = &mData.seriesData[ row ].second;
    }
    
    if ( *variable != value ) {
        mData.modified = true;
    }
    
    *variable = value;
}

void ExerciseWidget::setData(const ExerciseWidgetData &data)
{
    const bool modified = data.modified;
    mData = data;

    ui->lblExerciseName->setText(data.name);

    switch (data.type)
    {
    case Exercise::Repetition:
        ui->lblType->setText(trUtf8("Répétition"));
        break;
    case Exercise::Duration:
        ui->lblType->setText(trUtf8("Durée"));
        break;
    }

    switch (data.difficulty)
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

    ui->lblRest->setText(trUtf8("%1 secs.").arg(data.rest));
    ui->lblRepetitions->setNum(data.repetitions);
    ui->txtDescription->setPlainText(data.description);

    ui->lblNumber->setText(QString("#%1").arg(data.number));

    createLayout();
    
    mData.modified = modified;
}

const ExerciseWidgetData &ExerciseWidget::data() const
{
    return mData;
}

ExerciseWidget::PairSpinBox ExerciseWidget::addRow()
{
    const int rowCount = mPairs.count() +1;
    
    QLabel *lblTitle = new QLabel(trUtf8("Série %1").arg(rowCount));
    lblTitle->setObjectName(QString("lblTitle_%1").arg(rowCount-1));
    QSpinBox *txtResult = new QSpinBox;
    txtResult->setObjectName(QString("txtResult_%1").arg(rowCount-1));
    txtResult->setMinimum(0);
    txtResult->setMaximum(std::numeric_limits<int>::max());
    txtResult->setFixedWidth(40);
    QSpinBox *txtLoad = new QSpinBox;
    txtLoad->setObjectName(QString("txtLoad_%1").arg(rowCount-1));
    txtLoad->setMinimum(0);
    txtLoad->setMaximum(std::numeric_limits<int>::max());
    txtLoad->setFixedWidth(40);
    txtLoad->setEnabled(mData.weight);

    mPairs.append(qMakePair(txtResult, txtLoad));
    scrollAreaLayout->addWidget(lblTitle, rowCount, 0);
    scrollAreaLayout->addWidget(txtResult, rowCount, 1);
    scrollAreaLayout->addWidget(txtLoad, rowCount, 2);
    
    if (mData.seriesData.count() < mPairs.count())
    {
        mData.seriesData << qMakePair(txtResult->value(), txtLoad->value());
    }

    
    connect(txtResult ,SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged(int)));
    connect(txtLoad ,SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged(int)));
    
    return qMakePair(txtResult, txtLoad);
}

void ExerciseWidget::createLayout()
{
    delete scrollAreaLayout;
    qDeleteAll( ui->scrollAreaWidgetContents->findChildren<QLabel*>( QRegExp( "lblTitle_*" ) ) );
    foreach ( const ExerciseWidget::PairSpinBox& pair, mPairs ) {
        delete pair.first;
        delete pair.second;
    }
    mPairs.clear();

    /* Rebuild the layout */

    scrollAreaLayout = new QGridLayout;
    scrollAreaLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);
    scrollAreaLayout->setHorizontalSpacing(10);
    scrollAreaLayout->addWidget(new QLabel(trUtf8("Rés.")), 0, 1);
    scrollAreaLayout->addWidget(new QLabel(trUtf8("Charge")), 0, 2);
    
    const int count = qMax(mData.series, mData.seriesData.count());
    
    for (int i = 0; i < count; ++i)
    {
        const QPair<int, int> values = mData.seriesData.value(i);
        ExerciseWidget::PairSpinBox pair = addRow();
        pair.first->setValue(values.first);
        pair.second->setValue(values.second);

        if (i >= mData.series)
        {
            const QString labelName = QString("lblTitle_%1").arg(i);
            QLabel *lblSerie = ui->scrollAreaWidgetContents->findChild<QLabel *>(labelName);
            if (lblSerie)
            {
                const QString text = lblSerie->text();
                lblSerie->setText(
                       QString("<img src=\":/images/plus-icon.png\" width=12 height=12 />%1").arg(text));
            }
        }

    }

    ui->scrollAreaWidgetContents->setLayout(scrollAreaLayout);
}

void ExerciseWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange)
    {
        /* Enable or disable all loads lineEdits */
        QListIterator<ExerciseWidget::PairSpinBox> it(mPairs);
        while (it.hasNext())
        {
            it.next().second->setEnabled(mData.weight);
        }

        //ui->btnDeleteSerie->setEnabled(mPairs.count() > mData.series);
    }
}

/*void ExerciseWidget::selectResults(qint64 sessionMadeId)
{
    QSqlQuery query = SqlHelper::query();
    query.prepare("SELECT result, load, serie_number FROM session_made_result_view "
                  "WHERE id_session_made=:sessionMadeId AND id_session_exercise=:sessionExerciseId "
                  "ORDER BY serie_number");
    query.bindValue(":sessionMadeId", sessionMadeId);
    query.bindValue(":sessionExerciseId", mData.sessionExerciseId);

    if (query.exec())
    {
        while (query.next())
        {
            const int serieNumber = query.value(2).toInt();
            const int result = query.value(0).toInt();

            if (serieNumber >= mData.series)
            {
                addRow();
                const QString labelName = QString("lblTitle_%1").arg(serieNumber);
                QLabel *lblSerie = ui->scrollAreaWidgetContents->findChild<QLabel *>(labelName);
                if (lblSerie)
                {
                    const QString text = lblSerie->text();
                    lblSerie->setText(
                           QString("<img src=\":/images/plus-icon.png\" width=12 height=12 />%1").arg(text));
                }
            }

            const QString resultBoxName = QString("txtResult_%1").arg(serieNumber);
            QSpinBox *resultBox = ui->scrollAreaWidgetContents->findChild<QSpinBox *>(resultBoxName);
            if (resultBox)
            {
                resultBox->setValue(result);
                resultBox->setReadOnly(true);
                mData.seriesData[serieNumber].first = result;
            }
            else
            {
                qWarning() << Q_FUNC_INFO << " Couldn't find SpinBox " << resultBoxName;
            }

            if (mData.weight)
            {
                const int load = query.value(1).toInt();
                const QString loadBoxName = QString("txtLoad_%1").arg(serieNumber);
                QSpinBox *loadBox = ui->scrollAreaWidgetContents->findChild<QSpinBox *>(loadBoxName);
                if (loadBox)
                {
                    loadBox->setValue(load);
                    loadBox->setReadOnly(true);
                    mData.seriesData[serieNumber].second = load;
                }
                else
                {
                    qWarning() << Q_FUNC_INFO << " Couldn't find SpinBox " << loadBoxName;
                }
            }

        }
        ui->btnAddSerie->setEnabled(false);
    }
    else
    {
        qWarning() << Q_FUNC_INFO << " SQL Error: " << query.lastError();
    }
}*/
