#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui
{
class SettingsDialog;
}

class MainWindow;
class QAbstractButton;
class QSqlTableModel;
class QModelIndex;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(MainWindow *window);
    ~SettingsDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_btnAddMuscleGroup_clicked();
    void on_btnDeleteMuscleGroup_clicked();
    void on_lstMuscleGroups_clicked(const QModelIndex &index);
    void onSomethingChanged();

private:
    void writeSettings();
    void readSettings();
    void applySettings();

    Ui::SettingsDialog *ui;
    QSqlTableModel *mGroupsModel;
    MainWindow *mWindow;
    bool settingsChanged;

};

#endif // SETTINGSDIALOG_H
