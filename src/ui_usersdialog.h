/********************************************************************************
** Form generated from reading UI file 'usersdialog.ui'
**
** Created: Tue 1. Feb 17:19:36 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERSDIALOG_H
#define UI_USERSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UsersDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QComboBox *cmbUsers;
    QPushButton *btnDelete;
    QLineEdit *txtUser;
    QPushButton *btnAdd;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *spacer;
    QPushButton *btnStart;

    void setupUi(QDialog *UsersDialog)
    {
        if (UsersDialog->objectName().isEmpty())
            UsersDialog->setObjectName(QString::fromUtf8("UsersDialog"));
        UsersDialog->resize(284, 134);
        UsersDialog->setFocusPolicy(Qt::StrongFocus);
        verticalLayout = new QVBoxLayout(UsersDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        groupBox = new QGroupBox(UsersDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cmbUsers = new QComboBox(groupBox);
        cmbUsers->setObjectName(QString::fromUtf8("cmbUsers"));

        gridLayout->addWidget(cmbUsers, 0, 0, 1, 1);

        btnDelete = new QPushButton(groupBox);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));

        gridLayout->addWidget(btnDelete, 0, 1, 1, 1);

        txtUser = new QLineEdit(groupBox);
        txtUser->setObjectName(QString::fromUtf8("txtUser"));

        gridLayout->addWidget(txtUser, 1, 0, 1, 1);

        btnAdd = new QPushButton(groupBox);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setEnabled(false);

        gridLayout->addWidget(btnAdd, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacer);

        btnStart = new QPushButton(UsersDialog);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        btnStart->setFocusPolicy(Qt::StrongFocus);
        btnStart->setDefault(true);

        horizontalLayout->addWidget(btnStart);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(cmbUsers, txtUser);
        QWidget::setTabOrder(txtUser, btnAdd);
        QWidget::setTabOrder(btnAdd, btnStart);
        QWidget::setTabOrder(btnStart, btnDelete);

        retranslateUi(UsersDialog);

        QMetaObject::connectSlotsByName(UsersDialog);
    } // setupUi

    void retranslateUi(QDialog *UsersDialog)
    {
        UsersDialog->setWindowTitle(QApplication::translate("UsersDialog", "Bodeasy - D\303\251marrage", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("UsersDialog", "Utilisateurs", 0, QApplication::UnicodeUTF8));
        btnDelete->setText(QApplication::translate("UsersDialog", "Supprimer", 0, QApplication::UnicodeUTF8));
        btnAdd->setText(QApplication::translate("UsersDialog", "Ajouter", 0, QApplication::UnicodeUTF8));
        btnStart->setText(QApplication::translate("UsersDialog", "D\303\251marrer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UsersDialog: public Ui_UsersDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERSDIALOG_H
