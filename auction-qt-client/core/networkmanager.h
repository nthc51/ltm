#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "../models/user.h"
#include "../models/room.h"
#include "../models/auction.h"

class NetworkManager : public QObject
{
    Q_OBJECT
    
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    
    // Connection management
    void connectToServer(const QString& host, int port);
    void disconnectFromServer();
    bool isConnected() const;
    void sendRoomHistory(int roomId); 
    
    // Authentication
    void sendLogin(const QString& username, const QString& password);
    void sendRegister(const QString& username, const QString& password);
    void sendLogout(int userId);
    void sendSellerHistory(int userId);
    
    // Room management
    void sendCreateRoom(int userId, const QString& name, const QString& desc, 
                       int maxParticipants, int duration);
    void sendListRooms();
    void sendJoinRoom(int userId, int roomId);
    void sendLeaveRoom(int userId);
    
    // Auction management
    void sendCreateAuction(int sellerId, int roomId, const QString& title,
                          const QString& description, double startPrice,
                          double buyNowPrice, double minIncrement, int duration);
    void sendListAuctions(int roomId);
    void sendViewAuction(int auctionId);
    void sendDeleteAuction(int auctionId, int userId);
    void sendSearchAuctions(int roomId, const QString& keyword, 
                           double minPrice, double maxPrice);
    void sendActivateAuction(int auctionId, int sellerId);
    
    // Bidding
    void sendPlaceBid(int auctionId, int userId, double amount);
    void sendBuyNow(int auctionId, int userId);
    void sendBidHistory(int auctionId);
    
    // Account
    void sendRefresh(int userId);
    void sendAuctionHistory(int userId);
    
signals:
    // Connection signals
    void connected();
    void disconnected();
    void connectionError(const QString& error);
    void searchResultsReceived(const QString& results); 
    void auctionStarted(int auctionId);
    // Response signals
    void loginSuccess(User user);
    void loginFailed(const QString& reason);
    void registerSuccess();
    void registerFailed(const QString& reason);
    void logoutSuccess();
    void auctionDeletedBroadcast(int auctionId, QString title);
    void sellerHistoryReceived(const QString& history);
    void roomHistoryReceived(const QString& history);
    void roomCreated(int roomId);
    void roomListReceived(const QList<Room>& rooms);
    void joinedRoom(int roomId, const QString& roomName);
    void leftRoom();
    void roomError(const QString& error);
    
    void auctionCreated(int auctionId);
    void auctionListReceived(const QList<Auction>& auctions);
    void auctionDetails(const Auction& auction);
    void auctionDeleted();
    void auctionActivated();
    void auctionError(const QString& error);
    
    void bidPlaced();
    void bidError(const QString& error);
    void buyNowSuccess();
    void bidHistoryReceived(const QString& history);
    
    void balanceUpdated(double newBalance);
    void auctionHistoryReceived(const QString& history);
    
    // Real-time notifications
    void notification(const QString& type, const QString& message);
    void userJoinedRoom(const QString& username);
    void userLeftRoom(const QString& username);
    void newAuction(int auctionId, const QString& title);
    void newBid(int auctionId, double amount, const QString& bidder);
    void auctionWarning(int auctionId, int secondsLeft);
    void auctionEnded(int auctionId, const QString& winner, double finalPrice);
    void kicked(const QString& reason);
    
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    
private:
    QTcpSocket *socket;
    QString buffer;
    
    void sendCommand(const QString& command);
    void parseResponse(const QString& response);
    
    // Response parsers
    void parseLoginResponse(const QStringList& parts);
    void parseRoomListResponse(const QStringList& parts);
    void parseAuctionListResponse(const QStringList& parts);
    void parseNotification(const QStringList& parts);
};

#endif // NETWORKMANAGER_H
