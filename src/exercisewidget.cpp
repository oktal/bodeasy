#include "exercisewidget.h"
#include "ui_exercisewidget.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QFile>
#include <QDir>
#include <QDebug>

ExerciseWidget::ExerciseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExerciseWidget)
{
    ui->setupUi(this);
    for (int i = 0; i < 10; ++i)
    {
        addSerie();
    }
    ui->lblPixmap->setPixmap(QPixmap(qApp->applicationDirPath() + QDir::separator() +
                                     "exercice-triceps-08.gif"));
}

void ExerciseWidget::addSerie()
{
    int count = ui->seriesLayout->columnCount();
    QLabel *lblNumber = new QLabel(this);
    lblNumber->setFrameStyle(QFrame::Box);
    lblNumber->setAlignment(Qt::AlignCenter);
    lblNumber->setFixedSize(QSize(40, 32));
    lblNumber->setText(QString("%1").arg(count, 2, 10, QLatin1Char('0')));
    QLineEdit *txtSerie = new QLineEdit(this);
    txtSerie->setFixedWidth(40);
    txtSerie->setAlignment(Qt::AlignCenter);
    txtSerie->setValidator(new QIntValidator(this));
    ui->seriesLayout->addWidget(lblNumber, 0, count);
    ui->seriesLayout->addWidget(txtSerie, 1, count);
    if (count >= 8)
    {
        lblNumber->setEnabled(false);
        txtSerie->setEnabled(false);
    }
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}
