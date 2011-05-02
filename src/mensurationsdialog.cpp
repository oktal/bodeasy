#include "mensurationsdialog.h"
#include "ui_mensurationsdialog.h"
#include "ruleritem.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QFile>
#include <QSignalMapper>
#include <QPlainTextEdit>

#include <QDebug>

static const int Key = 0;

MensurationsDialog::MensurationsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MensurationsDialog),
    mScene(new QGraphicsScene)
{
    ui->setupUi(this);

    QGraphicsPixmapItem *manPixmap = new QGraphicsPixmapItem(
                QPixmap(":/images/fitness-man.png"));
    manPixmap->setPos(80, 0);

    RulerItem *thighRuler = new RulerItem(Qt::Horizontal, manPixmap);
    thighRuler->setWidth(80);
    thighRuler->setPos(100, 290);

    RulerItem *bicepsRuler = new RulerItem(Qt::Vertical, manPixmap);
    bicepsRuler->setWidth(55);
    bicepsRuler->setPos(60, 55);

    RulerItem *waistRuler = new RulerItem(Qt::Horizontal, manPixmap);
    waistRuler->setWidth(100);
    waistRuler->setPos(125, 195);

    RulerItem *pectoralRuler = new RulerItem(Qt::Horizontal, manPixmap);
    pectoralRuler->setWidth(160);
    pectoralRuler->setPos(80, 110);

    RulerItem *neckRuler = new RulerItem(Qt::Horizontal, manPixmap);
    neckRuler->setWidth(50);
    neckRuler->setPos(145, 65);

    RulerItem *calfRuler = new RulerItem(Qt::Horizontal, manPixmap);
    calfRuler->setWidth(50);
    calfRuler->setPos(160, 400);

    RulerItem *foreArmRuler = new RulerItem(Qt::Horizontal, manPixmap);
    foreArmRuler->setWidth(45);
    foreArmRuler->setPos(285, 80);

    RulerItem *ankleRuler = new RulerItem(Qt::Horizontal, manPixmap);
    ankleRuler->setWidth(25);
    ankleRuler->setPos(83, 440);

    RulerItem *wristRuler = new RulerItem(Qt::Horizontal, manPixmap);
    wristRuler->setWidth(25);
    wristRuler->setPos(290, 45);

    mScene->addItem(manPixmap);

    QIntValidator *validator = new QIntValidator(0, 1000, this);

    txtThigh = new QLineEdit;
    txtThigh->setFixedWidth(40);
    txtThigh->move(200, 265);
    txtThigh->setStyleSheet("border: 1px solid black;");
    txtThigh->setValidator(validator);
    thighRuler->setBuddy(txtThigh);

    txtBiceps = new QLineEdit;
    txtBiceps->setFixedWidth(40);
    txtBiceps->move(90, 50);
    txtBiceps->setStyleSheet("border: 1px solid black;");
    txtBiceps->setValidator(validator);
    bicepsRuler->setBuddy(txtBiceps);

    txtWaist = new QLineEdit;
    txtWaist->setFixedWidth(40);
    txtWaist->move(220, 170);
    txtWaist->setStyleSheet("border: 1px solid black;");
    txtWaist->setValidator(validator);
    waistRuler->setBuddy(txtWaist);

    txtPectoral = new QLineEdit;
    txtPectoral->setFixedWidth(40);
    txtPectoral->move(281, 123);
    txtPectoral->setStyleSheet("border: 1px solid black;");
    txtPectoral->setValidator(validator);
    pectoralRuler->setBuddy(txtPectoral);

    txtNeck = new QLineEdit;
    txtNeck->setFixedWidth(40);
    txtNeck->move(280, 50);
    txtNeck->setStyleSheet("border: 1px solid black;");
    txtNeck->setValidator(validator);
    neckRuler->setBuddy(txtNeck);

    txtCalf = new QLineEdit;
    txtCalf->setFixedWidth(40);
    txtCalf->move(250, 375);
    txtCalf->setStyleSheet("border: 1px solid black;");
    txtCalf->setValidator(validator);
    calfRuler->setBuddy(txtCalf);

    txtForeArm = new QLineEdit;
    txtForeArm->setFixedWidth(40);
    txtForeArm->move(420, 70);
    txtForeArm->setStyleSheet("border: 1px solid black;");
    txtForeArm->setValidator(validator);
    foreArmRuler->setBuddy(txtForeArm);

    txtWrist = new QLineEdit;
    txtWrist->setFixedWidth(40);
    txtWrist->move(400, 30);
    txtWrist->setStyleSheet("border: 1px solid black;");
    txtWrist->setValidator(validator);
    wristRuler->setBuddy(txtWrist);

    txtAnkle = new QLineEdit;
    txtAnkle->setFixedWidth(40);
    txtAnkle->move(160, 415);
    txtAnkle->setStyleSheet("border: 1px solid black;");
    txtAnkle->setValidator(validator);
    ankleRuler->setBuddy(txtAnkle);

    chkRulers = new QCheckBox(trUtf8("Afficher les règles"));
    chkRulers->setChecked(true);
    chkRulers->move(380, 480);
    connect(chkRulers, SIGNAL(clicked(bool)), this, SLOT(onChkRulersClicked(bool)));

    //mScene->setSceneRect(QRectF(QPointF(0, 0), QSizeF(400, 500)));
    mScene->addWidget(txtThigh);
    mScene->addWidget(txtBiceps);
    mScene->addWidget(txtWaist);
    mScene->addWidget(txtPectoral);
    mScene->addWidget(txtNeck);
    mScene->addWidget(txtCalf);
    mScene->addWidget(txtForeArm);
    mScene->addWidget(txtWrist);
    mScene->addWidget(txtAnkle);
    mScene->addWidget(chkRulers);

    ui->graphicsView->setSceneRect(QRectF(QPointF(0, 0), QSizeF(400, 500)));
    ui->graphicsView->setScene(mScene);
}

void MensurationsDialog::onChkRulersClicked(bool checked)
{
    foreach (QGraphicsItem *item, mScene->items())
    {
        if (item->type() == RulerItem::Type)
        {
            RulerItem *rulerItem = qgraphicsitem_cast<RulerItem *>(item);
            if (checked)
            {
                rulerItem->show();
            }
            else
            {
                rulerItem->hide();
            }
        }
    }
}
