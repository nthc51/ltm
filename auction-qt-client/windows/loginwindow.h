#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "../core/networkmanager.h"
#include "../models/user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_connectButton_clicked();
    
    // Network slots
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString& error);
    void onLoginSuccess(User user);
    void onLoginFailed(const QString& reason);
    void onRegisterSuccess();
    void onRegisterFailed(const QString& reason);

private:
    Ui::LoginWindow *ui;
    NetworkManager *network;
    
    void setUIEnabled(bool enabled);
    void showMainWindow(const User& user);
};

#endif // LOGINWINDOW_H
