#!/bin/bash

# Complete MainWindow Setup Script
# Run this in your auction-qt-client directory

echo "🔧 Creating MainWindow files..."

# Create mainwindow.cpp
cat > windows/mainwindow.cpp << 'MAINCPP'
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>
#include "../dialogs/bidplacedialog.h"
#include "../dialogs/createroomdialog.h"
#include "../dialogs/createauctiondialog.h"
#include "../utils/formatters.h"

MainWindow::MainWindow(NetworkManager *net, const User& user, QWidget *parent)
    : QMainWindow(parent), network(net), currentUser(user), countdownTimer(new QTimer(this))
{
    setupUI();
    connect(network, &NetworkManager::roomListReceived, this, &MainWindow::onRoomListReceived);
    connect(network, &NetworkManager::auctionListReceived, this, &MainWindow::onAuctionListReceived);
    connect(network, &NetworkManager::joinedRoom, this, &MainWindow::onJoinedRoom);
    connect(network, &NetworkManager::leftRoom, this, &MainWindow::onLeftRoom);
    connect(network, &NetworkManager::auctionCreated, this, &MainWindow::onAuctionCreated);
    connect(network, &NetworkManager::bidPlaced, this, &MainWindow::onBidPlaced);
    connect(network, &NetworkManager::buyNowSuccess, this, &MainWindow::onBuyNowSuccess);
    connect(network, &NetworkManager::balanceUpdated, this, &MainWindow::onBalanceUpdated);
    connect(network, &NetworkManager::notification, this, &MainWindow::onNotification);
    connect(network, &NetworkManager::newBid, this, &MainWindow::onNewBid);
    connect(network, &NetworkManager::auctionWarning, this, &MainWindow::onAuctionWarning);
    connect(network, &NetworkManager::auctionEnded, this, &MainWindow::onAuctionEnded);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdowns);
    countdownTimer->start(1000);
    network->sendListRooms();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("Auction System - " + currentUser.username);
    resize(1200, 800);
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    
    // Left panel
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QGroupBox *roomGroup = new QGroupBox("Rooms");
    QVBoxLayout *roomLayout = new QVBoxLayout(roomGroup);
    roomsList = new QListWidget();
    connect(roomsList, &QListWidget::itemDoubleClicked, this, &MainWindow::on_roomsList_itemDoubleClicked);
    roomLayout->addWidget(roomsList);
    
    QHBoxLayout *roomBtns = new QHBoxLayout();
    QPushButton *refreshRBtn = new QPushButton("Refresh");
    QPushButton *createRBtn = new QPushButton("Create");
    QPushButton *joinRBtn = new QPushButton("Join");
    QPushButton *leaveRBtn = new QPushButton("Leave");
    connect(refreshRBtn, &QPushButton::clicked, this, &MainWindow::on_refreshRoomsButton_clicked);
    connect(createRBtn, &QPushButton::clicked, this, &MainWindow::on_createRoomButton_clicked);
    connect(joinRBtn, &QPushButton::clicked, this, &MainWindow::on_joinRoomButton_clicked);
    connect(leaveRBtn, &QPushButton::clicked, this, &MainWindow::on_leaveRoomButton_clicked);
    roomBtns->addWidget(refreshRBtn);
    roomBtns->addWidget(createRBtn);
    roomBtns->addWidget(joinRBtn);
    roomBtns->addWidget(leaveRBtn);
    roomLayout->addLayout(roomBtns);
    leftLayout->addWidget(roomGroup);
    
    userInfoLabel = new QLabel();
    updateUserInfo();
    leftLayout->addWidget(userInfoLabel);
    
    // Right panel
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QGroupBox *auctionGroup = new QGroupBox("Auctions");
    QVBoxLayout *auctionLayout = new QVBoxLayout(auctionGroup);
    roomStatusLabel = new QLabel("Not in room");
    auctionLayout->addWidget(roomStatusLabel);
    auctionsList = new QListWidget();
    auctionLayout->addWidget(auctionsList);
    
    QHBoxLayout *aBtns = new QHBoxLayout();
    QPushButton *refreshABtn = new QPushButton("Refresh");
    QPushButton *createABtn = new QPushButton("Create");
    QPushButton *activateBtn = new QPushButton("Activate");
    QPushButton *deleteBtn = new QPushButton("Delete");
    connect(refreshABtn, &QPushButton::clicked, this, &MainWindow::on_refreshAuctionsButton_clicked);
    connect(createABtn, &QPushButton::clicked, this, &MainWindow::on_createAuctionButton_clicked);
    connect(activateBtn, &QPushButton::clicked, this, &MainWindow::on_activateAuctionButton_clicked);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteAuctionButton_clicked);
    aBtns->addWidget(refreshABtn);
    aBtns->addWidget(createABtn);
    aBtns->addWidget(activateBtn);
    aBtns->addWidget(deleteBtn);
    auctionLayout->addLayout(aBtns);
    
    QHBoxLayout *bidBtns = new QHBoxLayout();
    QPushButton *bidBtn = new QPushButton("Place Bid");
    QPushButton *buyBtn = new QPushButton("Buy Now");
    connect(bidBtn, &QPushButton::clicked, this, &MainWindow::on_placeBidButton_clicked);
    connect(buyBtn, &QPushButton::clicked, this, &MainWindow::on_buyNowButton_clicked);
    bidBtns->addWidget(bidBtn);
    bidBtns->addWidget(buyBtn);
    auctionLayout->addLayout(bidBtns);
    rightLayout->addWidget(auctionGroup);
    
    QGroupBox *logGroup = new QGroupBox("Activity Log");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    activityLog = new QTextEdit();
    activityLog->setReadOnly(true);
    activityLog->setMaximumHeight(150);
    logLayout->addWidget(activityLog);
    rightLayout->addWidget(logGroup);
    
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 2);
    setCentralWidget(central);
    
    QPushButton *logoutBtn = new QPushButton("Logout");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
    statusBar()->addPermanentWidget(logoutBtn);
}

void MainWindow::updateUserInfo() {
    userInfoLabel->setText(QString("<b>%1</b><br>%2")
        .arg(currentUser.username)
        .arg(Formatters::formatCurrency(currentUser.balance)));
}

void MainWindow::updateRoomStatus() {
    roomStatusLabel->setText(currentUser.isInRoom() ? 
        "Room: " + currentUser.currentRoomName : "Not in room");
}

void MainWindow::addLogMessage(const QString& msg) {
    activityLog->append("[" + QTime::currentTime().toString() + "] " + msg);
}

void MainWindow::showError(const QString& title, const QString& msg) {
    QMessageBox::warning(this, title, msg);
    addLogMessage("ERROR: " + msg);
}

void MainWindow::showSuccess(const QString& title, const QString& msg) {
    QMessageBox::information(this, title, msg);
}

Room MainWindow::getSelectedRoom() const {
    int row = roomsList->currentRow();
    return (row >= 0 && row < rooms.size()) ? rooms[row] : Room();
}

Auction MainWindow::getSelectedAuction() const {
    int row = auctionsList->currentRow();
    return (row >= 0 && row < auctions.size()) ? auctions[row] : Auction();
}

void MainWindow::on_refreshRoomsButton_clicked() {
    network->sendListRooms();
}

void MainWindow::on_createRoomButton_clicked() {
    CreateRoomDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        network->sendCreateRoom(currentUser.userId, dlg.getRoomName(),
            dlg.getDescription(), dlg.getMaxParticipants(), dlg.getDuration());
    }
}

void MainWindow::on_joinRoomButton_clicked() {
    Room r = getSelectedRoom();
    if (r.roomId > 0) network->sendJoinRoom(currentUser.userId, r.roomId);
    else showError("Error", "Select a room");
}

void MainWindow::on_leaveRoomButton_clicked() {
    if (currentUser.isInRoom()) network->sendLeaveRoom(currentUser.userId);
}

void MainWindow::on_refreshAuctionsButton_clicked() {
    if (currentUser.isInRoom()) network->sendListAuctions(currentUser.currentRoomId);
}

void MainWindow::on_createAuctionButton_clicked() {
    if (!currentUser.isInRoom()) { showError("Error", "Join room first"); return; }
    CreateAuctionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        network->sendCreateAuction(currentUser.userId, currentUser.currentRoomId,
            dlg.getTitle(), dlg.getDescription(), dlg.getStartPrice(),
            dlg.getBuyNowPrice(), dlg.getMinIncrement(), dlg.getDuration());
    }
}

void MainWindow::on_placeBidButton_clicked() {
    Auction a = getSelectedAuction();
    if (a.auctionId == 0) { showError("Error", "Select auction"); return; }
    if (!a.canBid()) { showError("Error", "Not active"); return; }
    BidPlaceDialog dlg(a, this);
    if (dlg.exec() == QDialog::Accepted) {
        network->sendPlaceBid(a.auctionId, currentUser.userId, dlg.getBidAmount());
    }
}

void MainWindow::on_buyNowButton_clicked() {
    Auction a = getSelectedAuction();
    if (a.auctionId == 0 || !a.hasBuyNow()) { showError("Error", "Invalid"); return; }
    if (QMessageBox::question(this, "Confirm", "Buy now?") == QMessageBox::Yes) {
        network->sendBuyNow(a.auctionId, currentUser.userId);
    }
}

void MainWindow::on_deleteAuctionButton_clicked() {
    Auction a = getSelectedAuction();
    if (a.auctionId > 0) network->sendDeleteAuction(a.auctionId, currentUser.userId);
}

void MainWindow::on_activateAuctionButton_clicked() {
    Auction a = getSelectedAuction();
    if (a.auctionId > 0) network->sendActivateAuction(a.auctionId, currentUser.userId);
}

void MainWindow::on_logoutButton_clicked() {
    network->sendLogout(currentUser.userId);
    close();
}

void MainWindow::on_roomsList_itemDoubleClicked(QListWidgetItem*) {
    on_joinRoomButton_clicked();
}

void MainWindow::on_roomsList_itemSelectionChanged() {}
void MainWindow::on_auctionsList_itemSelectionChanged() {}

void MainWindow::onRoomListReceived(const QList<Room>& r) {
    rooms = r;
    roomsList->clear();
    for (const Room& room : rooms) roomsList->addItem(room.getDisplayText());
    addLogMessage(QString("Loaded %1 rooms").arg(rooms.size()));
}

void MainWindow::onJoinedRoom(int id, const QString& name) {
    currentUser.currentRoomId = id;
    currentUser.currentRoomName = name;
    updateRoomStatus();
    addLogMessage("Joined: " + name);
    network->sendListAuctions(id);
}

void MainWindow::onLeftRoom() {
    currentUser.currentRoomId = 0;
    currentUser.currentRoomName.clear();
    updateRoomStatus();
    auctionsList->clear();
}

void MainWindow::onAuctionListReceived(const QList<Auction>& a) {
    auctions = a;
    auctionsList->clear();
    for (const Auction& auc : auctions) {
        auctionsList->addItem(QString("%1 | %2 | %3")
            .arg(auc.title)
            .arg(Formatters::formatCurrency(auc.currentPrice))
            .arg(auc.getStatusText()));
    }
}

void MainWindow::onAuctionCreated(int) {
    showSuccess("Success", "Created! Remember to activate");
    on_refreshAuctionsButton_clicked();
}

void MainWindow::onBidPlaced() {
    showSuccess("Success", "Bid placed!");
    on_refreshAuctionsButton_clicked();
}

void MainWindow::onBuyNowSuccess() {
    showSuccess("Success", "Purchased!");
    on_refreshAuctionsButton_clicked();
}

void MainWindow::onBalanceUpdated(double bal) {
    currentUser.balance = bal;
    updateUserInfo();
}

void MainWindow::onNotification(const QString& type, const QString& msg) {
    addLogMessage("[" + type + "] " + msg);
}

void MainWindow::onNewBid(int, double amt, const QString& bidder) {
    addLogMessage("New bid: " + Formatters::formatCurrency(amt) + " by " + bidder);
    on_refreshAuctionsButton_clicked();
}

void MainWindow::onAuctionWarning(int, int sec) {
    addLogMessage(QString("⚠️ Ending in %1s!").arg(sec));
}

void MainWindow::onAuctionEnded(int, const QString& winner, double price) {
    addLogMessage("🎉 Winner: " + winner + " at " + Formatters::formatCurrency(price));
    on_refreshAuctionsButton_clicked();
}

void MainWindow::updateCountdowns() {
    if (currentUser.isInRoom() && !auctions.isEmpty()) {
        onAuctionListReceived(auctions);
    }
}
MAINCPP

# Create simple mainwindow.ui
cat > windows/mainwindow.ui << 'MAINUI'
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect><x>0</x><y>0</y><width>1200</width><height>800</height></rect>
  </property>
  <widget class="QWidget" name="centralwidget"/>
 </widget>
 <resources/>
 <connections/>
</ui>
MAINUI

echo "✅ MainWindow files created!"
echo ""
echo "Now run:"
echo "  qmake AuctionClientQt.pro"
echo "  make"
echo "  ./AuctionClientQt"
SCRIPT

chmod +x /tmp/setup_mainwindow.sh
cat /tmp/setup_mainwindow.sh
