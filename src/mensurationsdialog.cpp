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
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QSqlError>
#include <QHash>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QFontMetrics>

#include <QDebug>

static QHash<QWidget *, QGraphicsTextItem *> labelsHash;

MensurationsDialog::MensurationsDialog(quint64 userId, QWidget *parent) :
    QDialog(parent),
    mUserId(userId),
    ui(new Ui::MensurationsDialog),
    mScene(new QGraphicsScene),
    mState(View)
{
    ui->setupUi(this);

    /* WARNING : widgets have to be created before rulers ! */
    createWidgets();
    createRulers();
    createLabels();
    setupMapper();

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
            if (proxyItem->widget() != btnAdd && proxyItem->widget() != btnCancel)
            {
                proxyItem->setZValue(2);
            }
            else
            {
                proxyItem->setZValue(1);
            }
            if (QLineEdit *edit = qobject_cast<QLineEdit *>(proxyItem->widget()))
            {
                edit->setEnabled(false);
                edit->setStyleSheet("border: 1px solid black;");
                edit->setValidator(validator);
                edit->setAlignment(Qt::AlignCenter);
                connect(edit, SIGNAL(textChanged(QString)), this,
                        SLOT(onLineEditChanged()));
            }
        }
    }

    mapper->toLast();
    cmbDates->setModel(model);
    cmbDates->setModelColumn(model->fieldIndex("date"));
    cmbDates->setCurrentIndex(mapper->currentIndex());
    connect(cmbDates, SIGNAL(currentIndexChanged(int)),
            mapper, SLOT(setCurrentIndex(int)));
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

    txtDescription = new QPlainTextEdit;
    txtDescription->move(300, 320);
    txtDescription->setFixedSize(200, 100);
    txtDescription->setReadOnly(true);
    txtDescription->setStyleSheet("border: 1px solid black;");

    QGraphicsSimpleTextItem *weightText = new
            QGraphicsSimpleTextItem(tr("Poids:"));
    weightText->setPos(350, 200);
    weightText->setZValue(1);
    txtWeight = new QLineEdit;
    txtWeight->setFixedWidth(40);
    txtWeight->move(390, 195);
    QGraphicsSimpleTextItem *weightUnit = new
            QGraphicsSimpleTextItem(tr("Kg."));
    weightUnit->setPos(435, 200);

    QGraphicsSimpleTextItem *sizeText = new
            QGraphicsSimpleTextItem(tr("Taille:"));
    sizeText->setPos(350, 240);
    txtSize = new QLineEdit;
    txtSize->setFixedWidth(40);
    txtSize->move(390, 235);
    QGraphicsSimpleTextItem *sizeUnit = new
            QGraphicsSimpleTextItem(tr("cm."));
    sizeUnit->setPos(435, 240);

    chkRulers = new QCheckBox(trUtf8("Afficher les règles"));
    chkRulers->setChecked(true);
    chkRulers->move(480, 460);
    connect(chkRulers, SIGNAL(clicked(bool)), this, SLOT(onChkRulersClicked(bool)));

    chkDescriptions = new QCheckBox(trUtf8("Afficher les indications"));
    chkDescriptions->setChecked(true);
    chkDescriptions->move(480, 480);
    connect(chkDescriptions, SIGNAL(clicked(bool)),
            txtDescription, SLOT(setVisible(bool)));

    cmbDates = new QComboBox;
    cmbDates->move(520, 20);
    cmbDates->setMinimumSize(100, 30);

    btnAdd = new QPushButton(QIcon(":/images/plus-icon.png"), tr("Ajouter"));
    btnAdd->setFixedSize(100, 30);
    btnAdd->move(520, 70);
    //btnAdd->move(0, 40);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));

    btnCancel = new QPushButton(QIcon(":/images/cancel-icon.png"), tr("Annuler"));
    btnCancel->setFixedSize(100, 30);
    btnCancel->move(520, 105);
    btnCancel->hide();
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(onBtnCancelClicked()));

    QGraphicsTextItem *visualisation = new QGraphicsTextItem(tr("Visualisation"));
    visualisation->setPos(450, 20);

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
    mScene->addItem(weightText);
    mScene->addItem(weightUnit);
    mScene->addWidget(txtWeight);
    mScene->addItem(sizeText);
    mScene->addItem(sizeUnit);
    mScene->addWidget(txtSize);
    mScene->addWidget(chkRulers);
    mScene->addWidget(chkDescriptions);
    mScene->addWidget(cmbDates);
    mScene->addWidget(btnAdd);
    mScene->addWidget(btnCancel);
    mScene->addWidget(txtDescription);
}

void MensurationsDialog::createLabels()
{
    QFontMetrics fm(font());
    foreach (QGraphicsItem *item, mScene->items())
    {
        if (item->type() == QGraphicsProxyWidget::Type)
        {
            QGraphicsProxyWidget *proxyItem = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
            if (QLineEdit *edit = qobject_cast<QLineEdit *>(proxyItem->widget()))
            {
                QGraphicsTextItem *txtDifference = new QGraphicsTextItem();
                txtDifference->setPos(edit->pos().x() + (edit->width() / 2) - fm.width("+9") / 2,
                                      edit->pos().y() - fm.height() - 5);
                mScene->addItem(txtDifference);
                labelsHash[edit] = txtDifference;
            }
        }
    }
}

/*!
  * \brief setup and configure the datawidgetmapper
*/
void MensurationsDialog::setupMapper()
{
    model = new QSqlTableModel(this);
    model->setTable("mensuration");
    model->setSort(model->fieldIndex("date"), Qt::AscendingOrder);
    model->select();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    /* Map all fields */
    mapper->addMapping(txtNeck, model->fieldIndex("neck"));
    mapper->addMapping(txtShoulder, model->fieldIndex("shoulder"));
    mapper->addMapping(txtPectoral, model->fieldIndex("chest"));
    mapper->addMapping(txtWaist, model->fieldIndex("waist_size"));
    mapper->addMapping(txtForeArm, model->fieldIndex("forearm"));
    mapper->addMapping(txtThigh, model->fieldIndex("thigh"));
    mapper->addMapping(txtCalf, model->fieldIndex("calf"));
    mapper->addMapping(txtBiceps, model->fieldIndex("biceps"));
    mapper->addMapping(txtWrist, model->fieldIndex("wrist"));
    mapper->addMapping(txtAnkle, model->fieldIndex("ankle"));
    mapper->addMapping(txtWeight, model->fieldIndex("weight"));
    mapper->addMapping(txtSize, model->fieldIndex("size"));
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
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

/*!
  * \brief Add or Validate a row into the model
*/
void MensurationsDialog::onBtnAddClicked()
{
    /* Check in which state we are */
    if (mState == View)
    {
        btnCancel->show();
        btnAdd->setText(tr("Valider"));
        btnAdd->setIcon(QIcon(":/images/check-icon.png"));
        enableEdits(true, true);
        mState = Add;
        model->insertRow(model->rowCount());
        mapper->toLast();
        model->setData(model->index(mapper->currentIndex(), model->fieldIndex("date")),
                       QDate::currentDate());
        model->setData(model->index(mapper->currentIndex(), model->fieldIndex("id_user")),
                       mUserId);
        cmbDates->setCurrentIndex(model->rowCount() - 1);
        cmbDates->setEnabled(false);
        btnAdd->setEnabled(true);
    }
    else if (mState == Add)
    {
        btnAdd->setText(tr("Ajouter"));
        btnAdd->setIcon(QIcon(":/images/plus-icon.png"));
        btnCancel->hide();
        enableEdits(false);
        mState = View;
        if (!mapper->submit())
        {
            qDebug() << model->lastError();
        }
        mapper->toLast();
        cmbDates->setCurrentIndex(model->rowCount() - 1);
        cmbDates->setEnabled(true);
    }
}

void MensurationsDialog::onBtnCancelClicked()
{
    model->removeRow(model->rowCount() - 1);
    mapper->toLast();
    mState = View;
    enableEdits(false);
    btnCancel->hide();
    cmbDates->setEnabled(true);
    cmbDates->setCurrentIndex(model->rowCount() - 1);
    btnAdd->setText(tr("Ajouter"));
    btnAdd->setIcon(QIcon(":/images/plus-icon.png"));
}

/*!
  * \brief Change labels associated with edits when
   any lineedit has changed
*/
void MensurationsDialog::onLineEditChanged()
{

    QLineEdit *edit = qobject_cast<QLineEdit *>(sender());
    if (!edit)
    {
        return;
    }

    QGraphicsTextItem *text = labelsHash[edit];
    if (model->rowCount() == 1 || mapper->currentIndex() == 0)
    {
        text->setPlainText("");
        return;
    }

    if (!edit->text().isEmpty())
    {
        /* Model's section of the edit */
        int section = mapper->mappedSection(edit);
        QVariant lastValue = model->data(model->index(mapper->currentIndex() - 1,
                                                      section));
        int delta = edit->text().toInt() - lastValue.toInt();
        QColor color;
        QString value = QString::number(delta);
        if (delta < 0)
        {
            color = Qt::red;
        }
        else if (delta == 0)
        {
            color = Qt::blue;
        }
        else
        {
            color = Qt::darkGreen;
            value.prepend("+");
        }
        text->setHtml(
              QString("<strong><font color=%1>%2</font></strong>").arg(color.name()).arg(value));
    }
    else
    {
        text->setPlainText("");
    }
}

/*!
  * \brief enable or not line edits
  * \param enable if true enable lineedits, else disable edits
  * \param clear if set to true, clear lineedits
      The default is false
*/

void MensurationsDialog::enableEdits(bool enable, bool clear)
{
    /* Loop over scene items and treat graphics proxy widgets */
    foreach (QGraphicsItem *item, mScene->items())
    {
        if (item->type() == QGraphicsProxyWidget::Type)
        {
            QGraphicsProxyWidget *proxyItem = qgraphicsitem_cast<QGraphicsProxyWidget *>(item);
            if (QLineEdit *edit = qobject_cast<QLineEdit *>(proxyItem->widget()))
            {
                edit->setEnabled(enable);
                if (clear)
                {
                    edit->clear();
                }
            }
        }
    }
}
