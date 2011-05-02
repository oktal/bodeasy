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

private:
    Ui::MensurationsDialog *ui;
    QGraphicsScene *mScene;

    QLineEdit *txtThigh;
    QLineEdit *txtBiceps;
    QLineEdit *txtWaist;
    QLineEdit *txtPectoral;
    QLineEdit *txtNeck;
    QLineEdit *txtCalf;
    QLineEdit *txtForeArm;
    QLineEdit *txtAnkle;
    QLineEdit *txtWrist;
    QCheckBox *chkRulers;
};

#endif // MENSURATIONSDIALOG_H
