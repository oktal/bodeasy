#ifndef MENSURATIONSDIALOG_H
#define MENSURATIONSDIALOG_H

#include <QDialog>
#include <QHash>

class QLineEdit;
class QCheckBox;
class QPlainTextEdit;

namespace Ui
{
    class MensurationsDialog;
}

class QGraphicsScene;

class MensurationsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MensurationsDialog(QWidget *parent = 0);

private slots:
    void onChkRulersClicked(bool checked);
    void onShowDescription(const QString &description);

private:
    enum State { View, Add };

    void createRulers();
    void createWidgets();

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
    QCheckBox *chkRulers;
    QPlainTextEdit *txtDescription;
};

#endif // MENSURATIONSDIALOG_H
