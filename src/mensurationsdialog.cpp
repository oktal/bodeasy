#include "mensurationsdialog.h"
#include "ui_mensurationsdialog.h"
#include "ruleritem.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QSignalMapper>
#include <QPlainTextEdit>
#include <QDate>

#include <QDebug>

MensurationsDialog::MensurationsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MensurationsDialog),
    mScene(new QGraphicsScene),
    mState(View)
{
    ui->setupUi(this);

    /* WARNING : widgets have to be created before rulers ! */
    createWidgets();
    createRulers();

    mScene->setSceneRect(QRectF(QPointF(0, 0), QSizeF(400, 500)));
    ui->graphicsView->setSceneRect(mScene->sceneRect());
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setScene(mScene);

    QIntValidator *validator = new QIntValidator(0, 1000, this);

    foreach (QGraphicsItem *item, mScene->items())
    {
        if (item->type() == RulerItem::Type)
        {
            RulerItem *rulerItem = qgraphicsitem_cast<RulerItem *>(item);
            connect(rulerItem, SIGNAL(showDescription(QString)), this,
                    SLOT(onShowDescription(QString)));
        }
        else if (item->type() == QGraphicsProxyWidget::Type)
        {
            QGraphicsProxyWidget *proxyItem = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
            proxyItem->setZValue(1);
            if (QLineEdit *edit = qobject_cast<QLineEdit *>(proxyItem->widget()))
            {
                edit->setEnabled(false);
                edit->setStyleSheet("border: 1px solid black;");
                edit->setValidator(validator);
            }
        }
    }
}

/*!
  * \brief Create and place all rulers
      Associate rulers with buddies (corresponding lineedits)
*/

void MensurationsDialog::createRulers()
{
    QGraphicsPixmapItem *manPixmap = new QGraphicsPixmapItem(
                QPixmap(":/images/fitness-man.png"));
    manPixmap->setPos(20, 0);

    RulerItem *thighRuler = new RulerItem(Qt::Horizontal, manPixmap);
    thighRuler->setWidth(80);
    thighRuler->setPos(100, 290);
    thighRuler->setDescription(
           trUtf8("<strong>Cuisse</strong>: décontracté, position debout, mesurer au plus gros se "
                   "situant sous la fesse."));
    thighRuler->setBuddy(txtThigh);

    RulerItem *bicepsRuler = new RulerItem(Qt::Vertical, manPixmap);
    bicepsRuler->setWidth(55);
    bicepsRuler->setPos(60, 55);
    bicepsRuler->setDescription(
            trUtf8("<strong>Biceps</strong>: bras contracté, levé parallèle au sol dans le même axe que les épaules, "
                   "avant-bras et poignets repliés ; mesurer au plus gros."));
    bicepsRuler->setBuddy(txtBiceps);

    RulerItem *waistRuler = new RulerItem(Qt::Horizontal, manPixmap);
    waistRuler->setWidth(100);
    waistRuler->setPos(125, 195);
    waistRuler->setDescription(
            trUtf8("<strong>Taille</strong>: décontracté, ne pas rentrer le ventre ni le sortir ; "
                   "mesurer au plus petit étroit, près du nombril."));
    waistRuler->setBuddy(txtWaist);

    RulerItem *pectoralRuler = new RulerItem(Qt::Horizontal, manPixmap);
    pectoralRuler->setWidth(160);
    pectoralRuler->setPos(80, 110);
    pectoralRuler->setDescription(
            trUtf8("<strong>Poitrine</strong>: Décontracté, respiration maintenue en position de repos "
                   "(ne pas bomber le torse), mesurer au niveau des tétons."));
    pectoralRuler->setBuddy(txtPectoral);

    RulerItem *shoulderRuler = new RulerItem(Qt::Horizontal, manPixmap);
    shoulderRuler->setWidth(190);
    shoulderRuler->setPos(75, 85);
    shoulderRuler->setDescription(
            trUtf8("<strong>Epaules</strong>: Décontracté, les bras se trouvent le longs du corps; "
                   "mesurer bien à l'horizontale juste en-dessous des clavicules."));
    shoulderRuler->setBuddy(txtShoulder);

    RulerItem *neckRuler = new RulerItem(Qt::Horizontal, manPixmap);
    neckRuler->setWidth(50);
    neckRuler->setPos(145, 65);
    neckRuler->setDescription(
           trUtf8("<strong>Cou</strong> : décontracté, mesurer en dessous la pomme d'Adam au plus petit endroit"));
    neckRuler->setBuddy(txtNeck);

    RulerItem *calfRuler = new RulerItem(Qt::Horizontal, manPixmap);
    calfRuler->setWidth(50);
    calfRuler->setPos(160, 400);
    calfRuler->setDescription(
           trUtf8("<strong>Mollet</strong>: décontracté, position debout en s'appuyant sur le talon, tendre la jambe; "
                  "mesurer au plus gros."));
    calfRuler->setBuddy(txtCalf);

    RulerItem *foreArmRuler = new RulerItem(Qt::Horizontal, manPixmap);
    foreArmRuler->setWidth(45);
    foreArmRuler->setPos(285, 80);
    foreArmRuler->setDescription(
                trUtf8("<strong>Avant Bras</strong>: Prenez la mesure sur la partie la plus large de vos avant-bras."));
    foreArmRuler->setBuddy(txtForeArm);

    RulerItem *ankleRuler = new RulerItem(Qt::Horizontal, manPixmap);
    ankleRuler->setWidth(25);
    ankleRuler->setPos(83, 440);
    ankleRuler->setDescription(
             trUtf8("<strong>Cuisse</strong>: debout, au plus gros (sous la fesse), décontracté ; "));
    ankleRuler->setBuddy(txtAnkle);

    RulerItem *wristRuler = new RulerItem(Qt::Horizontal, manPixmap);
    wristRuler->setWidth(25);
    wristRuler->setPos(290, 45);
    wristRuler->setDescription(
          trUtf8("<strong>Poignet</strong>: décontracté, main ouverte ; mesurer au plus petit, entre la main et "
                 "la petite protubérance osseuse de l'avant-bras."));
    wristRuler->setBuddy(txtWrist);

    mScene->addItem(manPixmap);
}

/*!
  * \brief Create and place all widgets
*/

void MensurationsDialog::createWidgets()
{
    txtThigh = new QLineEdit;
    txtThigh->setFixedWidth(40);
    txtThigh->move(140, 265);

    txtBiceps = new QLineEdit;
    txtBiceps->setFixedWidth(40);
    txtBiceps->move(30, 50);

    txtWaist = new QLineEdit;
    txtWaist->setFixedWidth(40);
    txtWaist->move(160, 170);

    txtPectoral = new QLineEdit;
    txtPectoral->setFixedWidth(40);
    txtPectoral->move(221, 123);

    txtShoulder = new QLineEdit;
    txtShoulder->setFixedWidth(40);
    txtShoulder->move(95, 57);

    txtNeck = new QLineEdit;
    txtNeck->setFixedWidth(40);
    txtNeck->move(220, 50);

    txtCalf = new QLineEdit;
    txtCalf->setFixedWidth(40);
    txtCalf->move(190, 375);

    txtForeArm = new QLineEdit;
    txtForeArm->setFixedWidth(40);
    txtForeArm->move(360, 70);

    txtWrist = new QLineEdit;
    txtWrist->setFixedWidth(40);
    txtWrist->move(340, 30);

    txtAnkle = new QLineEdit;
    txtAnkle->setFixedWidth(40);
    txtAnkle->move(100, 415);

    chkRulers = new QCheckBox(trUtf8("Afficher les règles"));
    chkRulers->setChecked(true);
    chkRulers->move(480, 480);
    connect(chkRulers, SIGNAL(clicked(bool)), this, SLOT(onChkRulersClicked(bool)));

    QComboBox *cmbDates = new QComboBox;
    cmbDates->setMinimumHeight(30);
    cmbDates->move(520, 20);
    cmbDates->addItem(QDate::currentDate().toString());

    QPushButton *btnAdd = new QPushButton(QIcon(":/images/plus-icon.png"), tr("Ajouter"));
    btnAdd->setFixedSize(100, 30);
    btnAdd->move(520, 70);

    QPushButton *btnCancel = new QPushButton(QIcon(":/images/cancel-icon.png"), tr("Annuler"));
    btnCancel->setFixedSize(100, 30);
    btnCancel->move(520, 105);
    btnCancel->hide();

    QGraphicsTextItem *visualisation = new QGraphicsTextItem(tr("Visualisation"));
    visualisation->setPos(450, 20);

    txtDescription = new QPlainTextEdit;
    txtDescription->move(300, 320);
    txtDescription->setFixedSize(200, 100);

    mScene->addItem(visualisation);
    mScene->addWidget(txtThigh);
    mScene->addWidget(txtBiceps);
    mScene->addWidget(txtWaist);
    mScene->addWidget(txtPectoral);
    mScene->addWidget(txtShoulder);
    mScene->addWidget(txtNeck);
    mScene->addWidget(txtCalf);
    mScene->addWidget(txtForeArm);
    mScene->addWidget(txtWrist);
    mScene->addWidget(txtAnkle);
    mScene->addWidget(chkRulers);
    mScene->addWidget(cmbDates);
    mScene->addWidget(btnAdd);
    mScene->addWidget(btnCancel);
    mScene->addWidget(txtDescription);
}

/*!
  * \brief Show or hide rulers
*/

void MensurationsDialog::onChkRulersClicked(bool checked)
{
    foreach (QGraphicsItem *item, mScene->items())
    {
        if (item->type() == RulerItem::Type)
        {
            item->setVisible(checked);
        }
    }
}

/*!
  * \brief Show the mensuration's description
*/

void MensurationsDialog::onShowDescription(const QString &description)
{
    txtDescription->clear();
    txtDescription->appendHtml(description);
}
