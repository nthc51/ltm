/********************************************************************************
** Form generated from reading UI file 'createauctiondialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEAUCTIONDIALOG_H
#define UI_CREATEAUCTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CreateAuctionDialog
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *titleEdit;
    QLabel *label1;
    QTextEdit *descEdit;
    QLabel *label2;
    QDoubleSpinBox *startPriceSpinBox;
    QLabel *label3;
    QDoubleSpinBox *buyNowPriceSpinBox;
    QLabel *label4;
    QDoubleSpinBox *minIncrementSpinBox;
    QLabel *label5;
    QSpinBox *durationSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateAuctionDialog)
    {
        if (CreateAuctionDialog->objectName().isEmpty())
            CreateAuctionDialog->setObjectName(QString::fromUtf8("CreateAuctionDialog"));
        CreateAuctionDialog->resize(400, 400);
        formLayout = new QFormLayout(CreateAuctionDialog);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(CreateAuctionDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        titleEdit = new QLineEdit(CreateAuctionDialog);
        titleEdit->setObjectName(QString::fromUtf8("titleEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, titleEdit);

        label1 = new QLabel(CreateAuctionDialog);
        label1->setObjectName(QString::fromUtf8("label1"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label1);

        descEdit = new QTextEdit(CreateAuctionDialog);
        descEdit->setObjectName(QString::fromUtf8("descEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, descEdit);

        label2 = new QLabel(CreateAuctionDialog);
        label2->setObjectName(QString::fromUtf8("label2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label2);

        startPriceSpinBox = new QDoubleSpinBox(CreateAuctionDialog);
        startPriceSpinBox->setObjectName(QString::fromUtf8("startPriceSpinBox"));
        startPriceSpinBox->setMaximum(999999999.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, startPriceSpinBox);

        label3 = new QLabel(CreateAuctionDialog);
        label3->setObjectName(QString::fromUtf8("label3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label3);

        buyNowPriceSpinBox = new QDoubleSpinBox(CreateAuctionDialog);
        buyNowPriceSpinBox->setObjectName(QString::fromUtf8("buyNowPriceSpinBox"));
        buyNowPriceSpinBox->setMaximum(999999999.000000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, buyNowPriceSpinBox);

        label4 = new QLabel(CreateAuctionDialog);
        label4->setObjectName(QString::fromUtf8("label4"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label4);

        minIncrementSpinBox = new QDoubleSpinBox(CreateAuctionDialog);
        minIncrementSpinBox->setObjectName(QString::fromUtf8("minIncrementSpinBox"));
        minIncrementSpinBox->setValue(100000.000000000000000);
        minIncrementSpinBox->setMaximum(999999999.000000000000000);

        formLayout->setWidget(4, QFormLayout::FieldRole, minIncrementSpinBox);

        label5 = new QLabel(CreateAuctionDialog);
        label5->setObjectName(QString::fromUtf8("label5"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label5);

        durationSpinBox = new QSpinBox(CreateAuctionDialog);
        durationSpinBox->setObjectName(QString::fromUtf8("durationSpinBox"));
        durationSpinBox->setMaximum(86400);
        durationSpinBox->setValue(180);

        formLayout->setWidget(5, QFormLayout::FieldRole, durationSpinBox);

        buttonBox = new QDialogButtonBox(CreateAuctionDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(6, QFormLayout::FieldRole, buttonBox);


        retranslateUi(CreateAuctionDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CreateAuctionDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CreateAuctionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CreateAuctionDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateAuctionDialog)
    {
        CreateAuctionDialog->setWindowTitle(QCoreApplication::translate("CreateAuctionDialog", "Create Auction", nullptr));
        label->setText(QCoreApplication::translate("CreateAuctionDialog", "Title:", nullptr));
        label1->setText(QCoreApplication::translate("CreateAuctionDialog", "Description:", nullptr));
        label2->setText(QCoreApplication::translate("CreateAuctionDialog", "Start Price (VND):", nullptr));
        label3->setText(QCoreApplication::translate("CreateAuctionDialog", "Buy Now Price (VND):", nullptr));
        label4->setText(QCoreApplication::translate("CreateAuctionDialog", "Min Increment (VND):", nullptr));
        label5->setText(QCoreApplication::translate("CreateAuctionDialog", "Duration (seconds):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateAuctionDialog: public Ui_CreateAuctionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEAUCTIONDIALOG_H
