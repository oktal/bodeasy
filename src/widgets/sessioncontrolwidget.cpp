#include "sessioncontrolwidget.h"
#include "ui_sessioncontrolwidget.h"

#include <QPushButton>
#include <QSignalMapper>

SessionControlWidget::SessionControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionControlWidget)
{
    ui->setupUi(this);
    mapper = new QSignalMapper(this);
    mapper->setMapping(ui->btnFirst, FirstButton);
    mapper->setMapping(ui->btnLast, LastButton);
    mapper->setMapping(ui->btnNext, NextButton);
    mapper->setMapping(ui->btnPrevious, PreviousButton);
    mapper->setMapping(ui->btnFinish, FinishButton);

    foreach (QPushButton *button, this->findChildren<QPushButton *>())
    {
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));
    connect(ui->chkObjectiveDone, SIGNAL(clicked(bool)), this, SIGNAL(objectiveClicked(bool)));
}

SessionControlWidget::~SessionControlWidget()
{
    delete ui;
}

void SessionControlWidget::setButtonEnabled(SessionControlWidget::Button button, bool enabled)
{
    QPushButton *b = qobject_cast<QPushButton *>(mapper->mapping(button));
    if (b)
    {
        b->setEnabled(enabled);
    }
}

bool SessionControlWidget::isButtonEnabled(SessionControlWidget::Button button) const
{
    QPushButton *b = qobject_cast<QPushButton *>(mapper->mapping(button));
    if (b)
    {
        return b->isEnabled();
    }

    return false;
}

void SessionControlWidget::setObjectiveVisible(bool visible)
{
    ui->wObjective->setVisible(visible);
}

bool SessionControlWidget::isObjectiveVisible() const
{
    return ui->wObjective->isVisible();
}

void SessionControlWidget::setObjectiveText(const QString &text)
{
    ui->lblObjective->setText(text);
}

QString SessionControlWidget::objectiveText() const
{
    return ui->lblObjective->text();
}

void SessionControlWidget::setObjectiveReadOnly(bool readOnly)
{
    ui->chkObjectiveDone->setDisabled(readOnly);
}

bool SessionControlWidget::isObjectiveReadOnly() const
{
    return !ui->chkObjectiveDone->isEnabled();
}

void SessionControlWidget::setObjectiveChecked(bool checked)
{
    ui->chkObjectiveDone->setChecked(checked);
}

bool SessionControlWidget::isObjectiveChecked() const
{
    return ui->chkObjectiveDone->isChecked();
}

void SessionControlWidget::onButtonClicked(int id)
{
    Button button = static_cast<Button>(id);
    switch (button)
    {
    case FirstButton:
        emit firstClicked();
        break;
    case LastButton:
        emit lastClicked();
        break;
    case PreviousButton:
        emit previousClicked();
        break;
    case NextButton:
        emit nextClicked();
        break;
    case FinishButton:
        emit finishClicked();
        break;
    }

    emit buttonClicked(button);
}

void SessionControlWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


