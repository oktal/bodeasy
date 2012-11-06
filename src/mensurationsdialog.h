#ifndef MENSURATIONSDIALOG_H
#define MENSURATIONSDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QPlainTextEdit;
class QComboBox;
class QDataWidgetMapper;
class QSqlTableModel;

namespace Ui
{
    class MensurationsDialog;
}

class QGraphicsScene;

class MensurationsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MensurationsDialog(quint64 userId, QWidget *parent = 0);

private slots:
    void onChkRulersClicked(bool checked);
    void onShowDescription(const QString &description);
    void onBtnAddClicked();
    void onBtnCancelClicked();
    void onLineEditChanged();

private:
    enum State { View, Add };

    void createRulers();
    void createWidgets();
    void createLabels();
    void setupMapper();
    void enableEdits(bool enable, bool clear = false);
    QGraphicsProxyWidget *addWidgetInScene(QWidget *widget, int x, int y);

    quint64 mUserId;
    Ui::MensurationsDialog *ui;
    QGraphicsScene *mScene;
    State mState;

    QLineEdit *txtThigh;
    QLineEdit *txtBiceps;
    QLineEdit *txtWaist;
    QLineEdit *txtPectoral;
    QLineEdit *txtShoulder;
    QLineEdit *txtNeck;
    QLineEdit *txtCalf;
    QLineEdit *txtForeArm;
    QLineEdit *txtAnkle;
    QLineEdit *txtWrist;
    QLineEdit *txtWeight;
    QLineEdit *txtSize;
    QCheckBox *chkRulers;
    QCheckBox *chkDescriptions;
    QPlainTextEdit *txtDescription;
    QComboBox *cmbDates;
    QPushButton *btnAdd;
    QPushButton *btnCancel;
    QDataWidgetMapper *mapper;
    QSqlTableModel *model;
};

#endif // MENSURATIONSDIALOG_H
