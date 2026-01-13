#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QListWidget>
#include <QTableWidget>       // ← THÊM
#include <QTableWidgetItem>   // ← THÊM
#include <QLabel>
#include <QHeaderView>       
#include <QTextEdit>
#include <QPushButton>
#include <QSet>               // ← THÊM
#include <QInputDialog>       // ← THÊM
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
    void on_roomsList_itemDoubleClicked(QListWidgetItem *item);
    void on_roomsList_itemSelectionChanged();
    void on_viewParticipatedHistoryButton_clicked();
    void on_viewSellerHistoryButton_clicked();
    void on_refreshAuctionsButton_clicked();
    void on_createAuctionButton_clicked();
    void on_viewAuctionDetailsButton_clicked();
    void on_placeBidButton_clicked();
     void onSearchResultsReceived(const QString& results);
    void on_buyNowButton_clicked();
void onAuctionStarted(int auctionId);
    void updateAuctionActionButtons();
    void on_deleteAuctionButton_clicked();
    void on_activateAuctionButton_clicked();
    void on_searchAuctionsButton_clicked();
    void on_bidHistoryButton_clicked();
    void on_auctionsList_itemSelectionChanged();
    void on_viewRoomHistoryButton_clicked();  
    void on_roomInfoButton_clicked();
    void on_viewHistoryButton_clicked();
    void on_logoutButton_clicked();
    void onRoomHistoryReceived(const QString& history); 
    void onSellerHistoryReceived(const QString& history); 
    void onRoomListReceived(const QList<Room>& rooms);
    void onJoinedRoom(int roomId, const QString& roomName);
    void onLeftRoom();
    void onRoomCreated(int roomId);
    void onAuctionListReceived(const QList<Auction>& auctions);
    void onAuctionCreated(int auctionId);
    void onAuctionActivated();
    void onBidPlaced();
    void onBuyNowSuccess();
    void onAuctionDeleted();
    void onAuctionDetails(const Auction& auction);
    void onBidHistoryReceived(const QString& history);
    void onAuctionHistoryReceived(const QString& history);
    void onBalanceUpdated(double newBalance);
    
    void onNotification(const QString& type, const QString& message);
    void onNewBid(int auctionId, double amount, const QString& bidder);
    void onNewAuction(int auctionId, const QString& title);
    void onAuctionWarning(int auctionId, int secondsLeft);
    void onAuctionEnded(int auctionId, const QString& winner, double finalPrice);
    void onUserJoinedRoom(const QString& username);
    void onUserLeftRoom(const QString& username);
    
    void onRoomError(const QString& error);
    void onAuctionError(const QString& error);
    void onBidError(const QString& error);
    void onDisconnected();
    
    void updateCountdowns();
    void checkAuctionWarnings();  // ← THÊM
    void onAuctionDeletedBroadcast(int auctionId, QString title);

private:
    NetworkManager *network;
    User currentUser;
    QList<Room> rooms;
    QList<Auction> auctions;
    QTimer *countdownTimer;
    QTimer *warningCheckTimer;    // ← THÊM
    QSet<int> warnedAuctions;     // ← THÊM
    
    
    QListWidget *roomsList;
    QListWidget *auctionsList;
    QListWidget *queueList;
    QLabel *userInfoLabel;
    QLabel *roomStatusLabel;
    QTextEdit *activityLog;
    
     QPushButton *bidButton;
    QPushButton *buyNowButton;
    QPushButton *deleteAuctionButton;
    void setupUI();
    void updateUserInfo();
    void updateRoomStatus();
    Room getSelectedRoom() const;
    Auction getSelectedAuction() const;
    bool userHasActiveBids();
    void addLogMessage(const QString& message, const QString& type = "INFO");
    void showError(const QString& title, const QString& message);
    void showSuccess(const QString& title, const QString& message);
    QPushButton* createStyledButton(const QString& text, const QString& color);
    QString adjustBrightness(const QString& color, int percent);
    
    void showBidHistoryDialog(const QString& history);
    void showAuctionEndedPopup(const Auction& auction, const QString& winner, double finalPrice);
};

#endif // MAINWINDOW_H