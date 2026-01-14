/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QGroupBox *serverGroupBox;
    QFormLayout *formLayout;
    QLabel *hostLabel;
    QLineEdit *hostEdit;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QPushButton *connectButton;
    QGroupBox *loginGroupBox;
    QFormLayout *formLayout_2;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *statusLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QString::fromUtf8("LoginWindow"));
        LoginWindow->resize(400, 300);
        verticalLayout = new QVBoxLayout(LoginWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleLabel = new QLabel(LoginWindow);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        serverGroupBox = new QGroupBox(LoginWindow);
        serverGroupBox->setObjectName(QString::fromUtf8("serverGroupBox"));
        formLayout = new QFormLayout(serverGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        hostLabel = new QLabel(serverGroupBox);
        hostLabel->setObjectName(QString::fromUtf8("hostLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, hostLabel);

        hostEdit = new QLineEdit(serverGroupBox);
        hostEdit->setObjectName(QString::fromUtf8("hostEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, hostEdit);

        portLabel = new QLabel(serverGroupBox);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, portLabel);

        portEdit = new QLineEdit(serverGroupBox);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, portEdit);

        connectButton = new QPushButton(serverGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        formLayout->setWidget(2, QFormLayout::FieldRole, connectButton);


        verticalLayout->addWidget(serverGroupBox);

        loginGroupBox = new QGroupBox(LoginWindow);
        loginGroupBox->setObjectName(QString::fromUtf8("loginGroupBox"));
        formLayout_2 = new QFormLayout(loginGroupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        usernameLabel = new QLabel(loginGroupBox);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameEdit = new QLineEdit(loginGroupBox);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));
        usernameEdit->setEnabled(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, usernameEdit);

        passwordLabel = new QLabel(loginGroupBox);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, passwordLabel);

        passwordEdit = new QLineEdit(loginGroupBox);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setEnabled(false);
        passwordEdit->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, passwordEdit);


        verticalLayout->addWidget(loginGroupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        loginButton = new QPushButton(LoginWindow);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setEnabled(false);

        horizontalLayout->addWidget(loginButton);

        registerButton = new QPushButton(LoginWindow);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setEnabled(false);

        horizontalLayout->addWidget(registerButton);


        verticalLayout->addLayout(horizontalLayout);

        statusLabel = new QLabel(LoginWindow);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(statusLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Login - Auction System", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginWindow", "<h2>\360\237\216\252 Auction System</h2>", nullptr));
        serverGroupBox->setTitle(QCoreApplication::translate("LoginWindow", "Server Connection", nullptr));
        hostLabel->setText(QCoreApplication::translate("LoginWindow", "Host:", nullptr));
        hostEdit->setText(QCoreApplication::translate("LoginWindow", "127.0.0.1", nullptr));
        portLabel->setText(QCoreApplication::translate("LoginWindow", "Port:", nullptr));
        portEdit->setText(QCoreApplication::translate("LoginWindow", "8080", nullptr));
        connectButton->setText(QCoreApplication::translate("LoginWindow", "Connect", nullptr));
        loginGroupBox->setTitle(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        usernameLabel->setText(QCoreApplication::translate("LoginWindow", "Username:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("LoginWindow", "Password:", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginWindow", "Register", nullptr));
        statusLabel->setText(QCoreApplication::translate("LoginWindow", "Not connected. Please connect to server first.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
