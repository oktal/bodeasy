#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include <QStyleFactory>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSettings>
#include <QSqlTableModel>

#include <QDebug>

#include "sql/models/usersmodel.h"
#include "sql/SqlHelper.h"
#include "mainwindow.h"
#include "SessionProxy.h"

SettingsDialog::SettingsDialog(MainWindow *window) :
    QDialog(window),
    ui(new Ui::SettingsDialog),
    mGroupsModel(new QSqlTableModel(this, SqlHelper::database())),
    mWindow(window)
{
    ui->setupUi(this);

    ui->cmbStyles->addItems(QStyleFactory::keys());
    ui->cmbExercisesView->setItemData(0, SessionProxy::Page);
    ui->cmbExercisesView->setItemData(1, SessionProxy::List);
    ui->cmbLanguage->setItemData(0, "fr_FR");
    ui->cmbLanguage->setItemData(1, "en_EN");

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    ui->cmbUsers->setModel(new UsersModel(this));
    mGroupsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mGroupsModel->setTable("muscle_group");
    mGroupsModel->setSort(mGroupsModel->fieldIndex("name"), Qt::AscendingOrder);
    mGroupsModel->select();
    ui->lstMuscleGroups->setModel(mGroupsModel);
    ui->lstMuscleGroups->setModelColumn(mGroupsModel->fieldIndex("name"));
    ui->lstMuscleGroups->setEditTriggers(QAbstractItemView::DoubleClicked);

    foreach (QCheckBox *checkBox, ui->page->findChildren<QCheckBox *>())
    {
        connect(checkBox, SIGNAL(clicked()), this, SLOT(onSomethingChanged()));
    }
    foreach(QComboBox *comboBox, ui->page_2->findChildren<QComboBox *>())
    {
        connect(comboBox, SIGNAL(activated(int)), this, SLOT(onSomethingChanged()));
    }
    settingsChanged = false;
    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Ok:
        if (settingsChanged)
        {
            applySettings();
        }
        accept();
    case QDialogButtonBox::Apply:
        applySettings();
        settingsChanged = false;
        button->setEnabled(false);
        break;
    case QDialogButtonBox::Cancel:
        reject();
        break;
    default:
        break;
    }
}

void SettingsDialog::on_btnAddMuscleGroup_clicked()
{
    onSomethingChanged();
    const int rowCount = mGroupsModel->rowCount();
    mGroupsModel->insertRow(rowCount);
    ui->lstMuscleGroups->edit(
                mGroupsModel->index(rowCount, mGroupsModel->fieldIndex("name")));
}

void SettingsDialog::on_btnDeleteMuscleGroup_clicked()
{
    onSomethingChanged();
    mGroupsModel->removeRow(ui->lstMuscleGroups->currentIndex().row());
}

void SettingsDialog::on_lstMuscleGroups_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    ui->btnDeleteMuscleGroup->setEnabled(true);
}

void SettingsDialog::applySettings()
{
    writeSettings();
    mGroupsModel->submitAll();
    qApp->setStyle(ui->cmbStyles->currentText());
    mWindow->reloadSettings();
}

void SettingsDialog::writeSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue(SETTING_WINDOW_SAVESTATE, ui->chkSaveWindowState->isChecked());
    settings.setValue(SETTING_START_CHRONO, ui->chkStartChrono->isChecked());
    settings.setValue(SETTING_RESET_CHRONO, ui->chkResetChrono->isChecked());
    settings.setValue(SETTING_STOP_CHRONO, ui->chkStopChrono->isChecked());
    settings.setValue(SETTING_AUTO_STARTUP, ui->chkAutoStart->isChecked());
    settings.setValue(SETTING_USER_STARTUP, ui->cmbUsers->currentText());
    settings.setValue(SETTING_LASTUSER_STARTUP, ui->chkSelectLastUser->isChecked());

    settings.setValue(SETTING_VIEW_STYLE, ui->cmbStyles->currentText());
    settings.setValue(SETTING_VIEW_MODE, ui->cmbExercisesView->itemData(
                          ui->cmbExercisesView->currentIndex()));
    settings.setValue(SETTING_VIEW_LANGUAGE, ui->cmbLanguage->itemData(
                          ui->cmbLanguage->currentIndex()));
}

void SettingsDialog::readSettings()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    const bool saveWindowState = settings.value(SETTING_WINDOW_SAVESTATE, true).toBool();
    const bool startChrono = settings.value(SETTING_START_CHRONO, true).toBool();
    const bool resetChrono = settings.value(SETTING_RESET_CHRONO, true).toBool();
    const bool stopChrono = settings.value(SETTING_STOP_CHRONO, true).toBool();
    const bool autoStartup = settings.value(SETTING_AUTO_STARTUP, false).toBool();
    const QString autoStartupUser = settings.value(SETTING_USER_STARTUP, QString()).toString();
    const bool lastUserStartup = settings.value(SETTING_LASTUSER_STARTUP, false).toBool();

    const QString style = settings.value(SETTING_VIEW_STYLE, qApp->style()->objectName()).toString();
    SessionProxy::ViewMode mode = static_cast<SessionProxy::ViewMode>(
                settings.value(SETTING_VIEW_MODE, SessionProxy::List).toInt());
    const QString locale = settings.value(SETTING_VIEW_LANGUAGE, "fr_FR").toString();

    ui->chkSaveWindowState->setChecked(saveWindowState);
    ui->chkStartChrono->setChecked(startChrono);
    ui->chkResetChrono->setChecked(resetChrono);
    ui->chkResetChrono->setEnabled(startChrono);
    ui->chkStopChrono->setChecked(stopChrono);
    ui->chkAutoStart->setChecked(autoStartup);
    ui->cmbUsers->setCurrentIndex(
                ui->cmbUsers->findText(autoStartupUser));
    ui->chkSelectLastUser->setChecked(lastUserStartup);
    ui->cmbStyles->setCurrentIndex(
                ui->cmbStyles->findText(style, Qt::MatchFixedString));
    ui->cmbExercisesView->setCurrentIndex(
                ui->cmbExercisesView->findData(mode));
    ui->cmbLanguage->setCurrentIndex(
                ui->cmbLanguage->findData(locale));
}

void SettingsDialog::onSomethingChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
    settingsChanged = true;
}
