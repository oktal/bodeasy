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
    QFrame(parent),
    ui(new Ui::ExerciseWidget),
    scrollAreaLayout(0),
    mReadOnly(false)
{
    ui->setupUi(this);
    
    foreach (QWidget* widget, findChildren<QWidget*>())
    {
        widget->setAttribute(Qt::WA_MacSmallSize);
    }
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

    ui->wMore->setVisible(!readOnly);
    ui->scrollArea->setVerticalScrollBarPolicy(readOnly ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAsNeeded);

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
    // do not use -1 because there is top labels in row 0
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
    lblTitle->setToolTip(lblTitle->text());
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
    
    lblTitle->setAttribute(Qt::WA_MacSmallSize);
    txtResult->setAttribute(Qt::WA_MacSmallSize);
    txtLoad->setAttribute(Qt::WA_MacSmallSize);

    mPairs.append(qMakePair(txtResult, txtLoad));
    scrollAreaLayout->addWidget(lblTitle, rowCount, 0);
    scrollAreaLayout->addWidget(txtResult, rowCount, 1);
    scrollAreaLayout->addWidget(txtLoad, rowCount, 2);
    
    if (mData.seriesData.count() < mPairs.count())
    {
        mData.seriesData << qMakePair(txtResult->value(), txtLoad->value());
    }

    if (rowCount-1 >= mData.series)
    {
        QPalette pal = lblTitle->palette();
        pal.setColor(lblTitle->foregroundRole(), QColor(0, 170, 0));
        lblTitle->setPalette(pal);
        lblTitle->setToolTip(QString("<img src=\":/images/plus-icon.png\" width=12 height=12 />%1").arg(lblTitle->text()));
    }
    
    connect(txtResult ,SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged(int)));
    connect(txtLoad ,SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged(int)));
    
    return qMakePair(txtResult, txtLoad);
}

void ExerciseWidget::createLayout()
{
    delete scrollAreaLayout;
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QLabel *>(QRegExp("lblTitle_*")));
    foreach (const ExerciseWidget::PairSpinBox &pair, mPairs)
    {
        delete pair.first;
        delete pair.second;
    }
    mPairs.clear();
    // delete dangling widgets (res, charge...)
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QLabel *>());

    /* Rebuild the layout */
    QLabel *lblRes = new QLabel(trUtf8("Rés."));
    lblRes->setAttribute(Qt::WA_MacSmallSize);
    lblRes->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QLabel *lblCharge = new QLabel(trUtf8("Charge"));
    lblCharge->setAttribute(Qt::WA_MacSmallSize);
    lblCharge->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    scrollAreaLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    scrollAreaLayout->setContentsMargins(QMargins(0, 0, 10, 0)); // 10 for right to avoid widgets to near of the vertical scrollbar
    scrollAreaLayout->setHorizontalSpacing(10);
    scrollAreaLayout->addWidget(lblRes, 0, 1, Qt::AlignCenter);
    scrollAreaLayout->addWidget(lblCharge, 0, 2, Qt::AlignCenter);
    QSpacerItem *item = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollAreaLayout->addItem(item, 10000, 0, 1, 3, Qt::AlignCenter);
    
    const int count = qMax(mData.series, mData.seriesData.count());
    
    for (int i = 0; i < count; ++i)
    {
        const QPair<int, int> values = mData.seriesData.value(i);
        ExerciseWidget::PairSpinBox pair = addRow();
        pair.first->setValue(values.first);
        pair.second->setValue(values.second);
    }
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
