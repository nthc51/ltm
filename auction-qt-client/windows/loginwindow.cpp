#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "../utils/constants.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
    , network(new NetworkManager(this))
{
    ui->setupUi(this);
    
    setWindowTitle(Constants::APP_NAME + " - Login");
    setFixedSize(400, 300);
    
    // Set default values
    ui->hostEdit->setText(Constants::DEFAULT_HOST);
    ui->portEdit->setText(QString::number(Constants::DEFAULT_PORT));
    
    // Connect network signals
    connect(network, &NetworkManager::connected, this, &LoginWindow::onConnected);
    connect(network, &NetworkManager::disconnected, this, &LoginWindow::onDisconnected);
    connect(network, &NetworkManager::connectionError, this, &LoginWindow::onConnectionError);
    connect(network, &NetworkManager::loginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(network, &NetworkManager::loginFailed, this, &LoginWindow::onLoginFailed);
    connect(network, &NetworkManager::registerSuccess, this, &LoginWindow::onRegisterSuccess);
    connect(network, &NetworkManager::registerFailed, this, &LoginWindow::onRegisterFailed);
    
    setUIEnabled(true);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_connectButton_clicked()
{
    QString host = ui->hostEdit->text().trimmed();
    int port = ui->portEdit->text().toInt();
    
    if (host.isEmpty() || port <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid host and port");
        return;
    }
    
    ui->statusLabel->setText("Connecting...");
    setUIEnabled(false);
    
    network->connectToServer(host, port);
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    
    if (username.length() < Constants::MIN_USERNAME_LENGTH) {
        QMessageBox::warning(this, "Invalid Input", 
            QString("Username must be at least %1 characters").arg(Constants::MIN_USERNAME_LENGTH));
        return;
    }
    
    if (password.length() < Constants::MIN_PASSWORD_LENGTH) {
        QMessageBox::warning(this, "Invalid Input",
            QString("Password must be at least %1 characters").arg(Constants::MIN_PASSWORD_LENGTH));
        return;
    }
    
    ui->statusLabel->setText("Logging in...");
    setUIEnabled(false);
    
    network->sendLogin(username, password);
}

void LoginWindow::on_registerButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    
    if (username.length() < Constants::MIN_USERNAME_LENGTH) {
        QMessageBox::warning(this, "Invalid Input",
            QString("Username must be at least %1 characters").arg(Constants::MIN_USERNAME_LENGTH));
        return;
    }
    
    if (password.length() < Constants::MIN_PASSWORD_LENGTH) {
        QMessageBox::warning(this, "Invalid Input",
            QString("Password must be at least %1 characters").arg(Constants::MIN_PASSWORD_LENGTH));
        return;
    }
    
    ui->statusLabel->setText("Registering...");
    setUIEnabled(false);
    
    network->sendRegister(username, password);
}

void LoginWindow::onConnected()
{
    ui->statusLabel->setText("Connected! Please login.");
    ui->statusLabel->setStyleSheet("color: green;");
    setUIEnabled(true);
    ui->connectButton->setEnabled(false);
}

void LoginWindow::onDisconnected()
{
    ui->statusLabel->setText("Disconnected from server");
    ui->statusLabel->setStyleSheet("color: red;");
    setUIEnabled(true);
    ui->connectButton->setEnabled(true);
}

void LoginWindow::onConnectionError(const QString& error)
{
    ui->statusLabel->setText("Connection error: " + error);
    ui->statusLabel->setStyleSheet("color: red;");
    setUIEnabled(true);
    QMessageBox::critical(this, "Connection Error", error);
}

void LoginWindow::onLoginSuccess(User user)
{
    ui->statusLabel->setText("Login successful!");
    ui->statusLabel->setStyleSheet("color: green;");
    showMainWindow(user);
}

void LoginWindow::onLoginFailed(const QString& reason)
{
    ui->statusLabel->setText("Login failed: " + reason);
    ui->statusLabel->setStyleSheet("color: red;");
    setUIEnabled(true);
    QMessageBox::warning(this, "Login Failed", reason);
}

void LoginWindow::onRegisterSuccess()
{
    ui->statusLabel->setText("Registration successful! Please login.");
    ui->statusLabel->setStyleSheet("color: green;");
    setUIEnabled(true);
    QMessageBox::information(this, "Success", "Registration successful! You can now login.");
}

void LoginWindow::onRegisterFailed(const QString& reason)
{
    ui->statusLabel->setText("Registration failed: " + reason);
    ui->statusLabel->setStyleSheet("color: red;");
    setUIEnabled(true);
    QMessageBox::warning(this, "Registration Failed", reason);
}

void LoginWindow::setUIEnabled(bool enabled)
{
    ui->usernameEdit->setEnabled(enabled);
    ui->passwordEdit->setEnabled(enabled);
    ui->loginButton->setEnabled(enabled && network->isConnected());
    ui->registerButton->setEnabled(enabled && network->isConnected());
}

void LoginWindow::showMainWindow(const User& user)
{
    MainWindow *mainWindow = new MainWindow(network, user);
    mainWindow->show();
    
    // Transfer ownership of network to main window
    network->setParent(mainWindow);
    
    accept(); // Close login window
}
