/********************************************************************************
** Form generated from reading UI file 'bidplacedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIDPLACEDIALOG_H
#define UI_BIDPLACEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BidPlaceDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *titleLabel;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *currentPriceLabel;
    QLabel *label1;
    QLabel *minBidLabel;
    QLabel *label2;
    QDoubleSpinBox *bidAmountSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BidPlaceDialog)
    {
        if (BidPlaceDialog->objectName().isEmpty())
            BidPlaceDialog->setObjectName(QString::fromUtf8("BidPlaceDialog"));
        BidPlaceDialog->resize(350, 200);
        vboxLayout = new QVBoxLayout(BidPlaceDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        titleLabel = new QLabel(BidPlaceDialog);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        titleLabel->setFont(font);

        vboxLayout->addWidget(titleLabel);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(BidPlaceDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        currentPriceLabel = new QLabel(BidPlaceDialog);
        currentPriceLabel->setObjectName(QString::fromUtf8("currentPriceLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, currentPriceLabel);

        label1 = new QLabel(BidPlaceDialog);
        label1->setObjectName(QString::fromUtf8("label1"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label1);

        minBidLabel = new QLabel(BidPlaceDialog);
        minBidLabel->setObjectName(QString::fromUtf8("minBidLabel"));

        formLayout->setWidget(1, QFormLayout::FieldRole, minBidLabel);

        label2 = new QLabel(BidPlaceDialog);
        label2->setObjectName(QString::fromUtf8("label2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label2);

        bidAmountSpinBox = new QDoubleSpinBox(BidPlaceDialog);
        bidAmountSpinBox->setObjectName(QString::fromUtf8("bidAmountSpinBox"));
        bidAmountSpinBox->setMaximum(999999999.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, bidAmountSpinBox);


        vboxLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(BidPlaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(BidPlaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), BidPlaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BidPlaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(BidPlaceDialog);
    } // setupUi

    void retranslateUi(QDialog *BidPlaceDialog)
    {
        BidPlaceDialog->setWindowTitle(QCoreApplication::translate("BidPlaceDialog", "Place Bid", nullptr));
        titleLabel->setText(QCoreApplication::translate("BidPlaceDialog", "Auction Title", nullptr));
        label->setText(QCoreApplication::translate("BidPlaceDialog", "Current Price:", nullptr));
        currentPriceLabel->setText(QCoreApplication::translate("BidPlaceDialog", "0 VND", nullptr));
        label1->setText(QCoreApplication::translate("BidPlaceDialog", "Minimum Bid:", nullptr));
        minBidLabel->setText(QCoreApplication::translate("BidPlaceDialog", "0 VND", nullptr));
        label2->setText(QCoreApplication::translate("BidPlaceDialog", "Your Bid:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BidPlaceDialog: public Ui_BidPlaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIDPLACEDIALOG_H
