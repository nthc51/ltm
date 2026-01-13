/********************************************************************************
** Form generated from reading UI file 'createroomdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEROOMDIALOG_H
#define UI_CREATEROOMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CreateRoomDialog
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *nameEdit;
    QLabel *label1;
    QTextEdit *descEdit;
    QLabel *label2;
    QSpinBox *maxParticipantsSpinBox;
    QLabel *label3;
    QSpinBox *durationSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateRoomDialog)
    {
        if (CreateRoomDialog->objectName().isEmpty())
            CreateRoomDialog->setObjectName(QString::fromUtf8("CreateRoomDialog"));
        CreateRoomDialog->resize(400, 300);
        formLayout = new QFormLayout(CreateRoomDialog);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(CreateRoomDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        nameEdit = new QLineEdit(CreateRoomDialog);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameEdit);

        label1 = new QLabel(CreateRoomDialog);
        label1->setObjectName(QString::fromUtf8("label1"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label1);

        descEdit = new QTextEdit(CreateRoomDialog);
        descEdit->setObjectName(QString::fromUtf8("descEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, descEdit);

        label2 = new QLabel(CreateRoomDialog);
        label2->setObjectName(QString::fromUtf8("label2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label2);

        maxParticipantsSpinBox = new QSpinBox(CreateRoomDialog);
        maxParticipantsSpinBox->setObjectName(QString::fromUtf8("maxParticipantsSpinBox"));
        maxParticipantsSpinBox->setMinimum(2);
        maxParticipantsSpinBox->setMaximum(100);
        maxParticipantsSpinBox->setValue(10);

        formLayout->setWidget(2, QFormLayout::FieldRole, maxParticipantsSpinBox);

        label3 = new QLabel(CreateRoomDialog);
        label3->setObjectName(QString::fromUtf8("label3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label3);

        durationSpinBox = new QSpinBox(CreateRoomDialog);
        durationSpinBox->setObjectName(QString::fromUtf8("durationSpinBox"));
        durationSpinBox->setMaximum(86400);
        durationSpinBox->setValue(3600);

        formLayout->setWidget(3, QFormLayout::FieldRole, durationSpinBox);

        buttonBox = new QDialogButtonBox(CreateRoomDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(4, QFormLayout::FieldRole, buttonBox);


        retranslateUi(CreateRoomDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CreateRoomDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CreateRoomDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CreateRoomDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateRoomDialog)
    {
        CreateRoomDialog->setWindowTitle(QCoreApplication::translate("CreateRoomDialog", "Create Room", nullptr));
        label->setText(QCoreApplication::translate("CreateRoomDialog", "Room Name:", nullptr));
        label1->setText(QCoreApplication::translate("CreateRoomDialog", "Description:", nullptr));
        label2->setText(QCoreApplication::translate("CreateRoomDialog", "Max Participants:", nullptr));
        label3->setText(QCoreApplication::translate("CreateRoomDialog", "Duration (seconds):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateRoomDialog: public Ui_CreateRoomDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEROOMDIALOG_H
