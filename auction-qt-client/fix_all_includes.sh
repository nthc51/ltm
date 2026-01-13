#!/bin/bash

echo "🔧 Fixing all missing includes and declarations..."

# 1. Fix mainwindow.h - Add widget members
cat > windows/mainwindow.h << 'MAINH'
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include "../core/networkmanager.h"
#include "../models/user.h"
#include "../models/room.h"
#include "../models/auction.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(NetworkManager *net, const User& user, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_refreshRoomsButton_clicked();
    void on_createRoomButton_clicked();
    void on_joinRoomButton_clicked();
    void on_leaveRoomButton_clicked();
    void on_refreshAuctionsButton_clicked();
    void on_createAuctionButton_clicked();
    void on_placeBidButton_clicked();
    void on_buyNowButton_clicked();
    void on_deleteAuctionButton_clicked();
    void on_activateAuctionButton_clicked();
    void on_logoutButton_clicked();
    void on_roomsList_itemDoubleClicked(QListWidgetItem *item);
    void on_roomsList_itemSelectionChanged();
    void on_auctionsList_itemSelectionChanged();
    void onRoomListReceived(const QList<Room>& rooms);
    void onJoinedRoom(int roomId, const QString& roomName);
    void onLeftRoom();
    void onAuctionListReceived(const QList<Auction>& auctions);
    void onAuctionCreated(int auctionId);
    void onBidPlaced();
    void onBuyNowSuccess();
    void onBalanceUpdated(double newBalance);
    void onNotification(const QString& type, const QString& message);
    void onNewBid(int auctionId, double amount, const QString& bidder);
    void onAuctionWarning(int auctionId, int secondsLeft);
    void onAuctionEnded(int auctionId, const QString& winner, double finalPrice);
    void updateCountdowns();

private:
    NetworkManager *network;
    User currentUser;
    QList<Room> rooms;
    QList<Auction> auctions;
    QTimer *countdownTimer;
    
    // UI widgets
    QListWidget *roomsList;
    QListWidget *auctionsList;
    QLabel *userInfoLabel;
    QLabel *roomStatusLabel;
    QTextEdit *activityLog;
    
    void setupUI();
    void updateUserInfo();
    void updateRoomStatus();
    Room getSelectedRoom() const;
    Auction getSelectedAuction() const;
    void addLogMessage(const QString& message);
    void showError(const QString& title, const QString& message);
    void showSuccess(const QString& title, const QString& message);
};

#endif // MAINWINDOW_H
MAINH

# 2. Fix mainwindow.cpp - Add QStatusBar include
sed -i '1a #include <QStatusBar>' windows/mainwindow.cpp
sed -i '1a #include <QTime>' windows/mainwindow.cpp

echo "✅ Fixed mainwindow.h - Added widget members"
echo "✅ Fixed mainwindow.cpp - Added QStatusBar and QTime includes"
