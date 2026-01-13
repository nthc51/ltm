#include "networkmanager.h"
#include "../utils/constants.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &NetworkManager::onError);
}

NetworkManager::~NetworkManager()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}

// ==================== CONNECTION ====================

void NetworkManager::connectToServer(const QString& host, int port)
{
    qDebug() << "Connecting to" << host << ":" << port;
    socket->connectToHost(host, port);
}

void NetworkManager::disconnectFromServer()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}

bool NetworkManager::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::onConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void NetworkManager::onDisconnected()
{
    qDebug() << "Disconnected from server";
    buffer.clear();
    emit disconnected();
}

void NetworkManager::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QString errorMsg = socket->errorString();
    qDebug() << "Socket error:" << errorMsg;
    emit connectionError(errorMsg);
}

// ==================== SEND COMMAND ====================

void NetworkManager::sendCommand(const QString& command)
{
    if (!isConnected()) {
        qDebug() << "Not connected! Cannot send:" << command;
        return;
    }
    
    QString fullCommand = command + Constants::LINE_END;
    qDebug() << "Sending:" << command;
    
    QByteArray data = fullCommand.toUtf8();
    qint64 written = socket->write(data);
    
    if (written == -1) {
        qDebug() << "Failed to write data";
        emit connectionError("Failed to send command");
    } else {
        socket->flush();
    }
}

// ==================== READ RESPONSE ====================

void NetworkManager::onReadyRead()
{
    QByteArray data = socket->readAll();
    buffer += QString::fromUtf8(data);
    
    // Process complete lines
    while (buffer.contains('\n')) {
        int newlinePos = buffer.indexOf('\n');
        QString line = buffer.left(newlinePos).trimmed();
        buffer = buffer.mid(newlinePos + 1);
        
        if (!line.isEmpty()) {
            qDebug() << "Received:" << line;
            parseResponse(line);
        }
    }
}

// ==================== AUTHENTICATION ====================

void NetworkManager::sendLogin(const QString& username, const QString& password)
{
    QString command = QString("LOGIN%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(username)
        .arg(password);
    sendCommand(command);
}

void NetworkManager::sendRegister(const QString& username, const QString& password)
{
    QString command = QString("REGISTER%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(username)
        .arg(password);
    sendCommand(command);
}

void NetworkManager::sendLogout(int userId)
{
    QString command = QString("LOGOUT%1%2")
        .arg(Constants::DELIMITER)
        .arg(userId);
    sendCommand(command);
}

// ==================== ROOM MANAGEMENT ====================

void NetworkManager::sendCreateRoom(int userId, const QString& name, 
                                   const QString& desc, int maxParticipants, 
                                   int duration)
{
    QString command = QString("CREATE_ROOM%1%2%1%3%1%4%1%5%1%6")
        .arg(Constants::DELIMITER)
        .arg(userId)
        .arg(name)
        .arg(desc)
        .arg(maxParticipants)
        .arg(duration);
    sendCommand(command);
}

void NetworkManager::sendListRooms()
{
    sendCommand("LIST_ROOMS|");
}

void NetworkManager::sendJoinRoom(int userId, int roomId)
{
    QString command = QString("JOIN_ROOM%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(userId)
        .arg(roomId);
    sendCommand(command);
}

void NetworkManager::sendLeaveRoom(int userId)
{
    QString command = QString("LEAVE_ROOM%1%2")
        .arg(Constants::DELIMITER)
        .arg(userId);
    sendCommand(command);
}

// ==================== AUCTION MANAGEMENT ====================

void NetworkManager::sendCreateAuction(int sellerId, int roomId, const QString& title,
                                      const QString& description, double startPrice,
                                      double buyNowPrice, double minIncrement, 
                                      int duration)
{
    QString command = QString("CREATE_AUCTION%1%2%1%3%1%4%1%5%1%6%1%7%1%8%1%9")
        .arg(Constants::DELIMITER)
        .arg(sellerId)
        .arg(roomId)
        .arg(title)
        .arg(description)
        .arg(startPrice, 0, 'f', 2)
        .arg(buyNowPrice, 0, 'f', 2)
        .arg(minIncrement, 0, 'f', 2)
        .arg(duration);
    sendCommand(command);
}

void NetworkManager::sendListAuctions(int roomId)
{
    QString command = QString("LIST_AUCTIONS%1%2")
        .arg(Constants::DELIMITER)
        .arg(roomId);
    sendCommand(command);
}

void NetworkManager::sendViewAuction(int auctionId)
{
    QString command = QString("VIEW_AUCTION%1%2")
        .arg(Constants::DELIMITER)
        .arg(auctionId);
    sendCommand(command);
}

void NetworkManager::sendDeleteAuction(int auctionId, int userId)
{
    QString command = QString("DELETE_AUCTION%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(auctionId)
        .arg(userId);
    sendCommand(command);
}

void NetworkManager::sendSearchAuctions(int roomId, const QString& keyword, 
                                       double minPrice, double maxPrice)
{
    QString command = QString("SEARCH_AUCTIONS%1%2%1%3%1%4%1%5")
        .arg(Constants::DELIMITER)
        .arg(roomId)
        .arg(keyword)
        .arg(minPrice, 0, 'f', 2)
        .arg(maxPrice, 0, 'f', 2);
    sendCommand(command);
}
void NetworkManager::sendRoomHistory(int roomId)
{
    QString command = QString("ROOM_HISTORY%1%2")
        .arg(Constants::DELIMITER)
        .arg(roomId);
    sendCommand(command);
}
void NetworkManager::sendActivateAuction(int auctionId, int sellerId)
{
    QString command = QString("ACTIVATE_AUCTION%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(auctionId)
        .arg(sellerId);
    sendCommand(command);
}

// ==================== BIDDING ====================

void NetworkManager::sendPlaceBid(int auctionId, int userId, double amount)
{
    QString command = QString("PLACE_BID%1%2%1%3%1%4")
        .arg(Constants::DELIMITER)
        .arg(auctionId)
        .arg(userId)
        .arg(amount, 0, 'f', 2);
    sendCommand(command);
}

void NetworkManager::sendBuyNow(int auctionId, int userId)
{
    QString command = QString("BUY_NOW%1%2%1%3")
        .arg(Constants::DELIMITER)
        .arg(auctionId)
        .arg(userId);
    sendCommand(command);
}

void NetworkManager::sendBidHistory(int auctionId)
{
    QString command = QString("BID_HISTORY%1%2")
        .arg(Constants::DELIMITER)
        .arg(auctionId);
    sendCommand(command);
}

// ==================== ACCOUNT ====================

void NetworkManager::sendRefresh(int userId)
{
    QString command = QString("REFRESH%1%2")
        .arg(Constants::DELIMITER)
        .arg(userId);
    sendCommand(command);
}

void NetworkManager::sendAuctionHistory(int userId)
{
    QString command = QString("AUCTION_HISTORY%1%2")
        .arg(Constants::DELIMITER)
        .arg(userId);
    sendCommand(command);
}

// ==================== PARSE RESPONSE ====================

void NetworkManager::parseResponse(const QString& response)
{
    QStringList parts = response.split(Constants::DELIMITER);
    if (parts.isEmpty()) return;
    
    QString command = parts[0];
    
    // Authentication responses
    if (command == "LOGIN_SUCCESS") {
        parseLoginResponse(parts);
    }
    else if (command == "LOGIN_FAIL") {
        emit loginFailed(parts.value(1, "Invalid credentials"));
    }
    else if (command == "REGISTER_SUCCESS") {
        // Server: REGISTER_SUCCESS|userId|username
        emit registerSuccess();
    }
    
    else if (command == "REGISTER_FAIL") {
        emit registerFailed(parts.value(1, "Registration failed"));
    }
    else if (command == "LOGOUT_SUCCESS") {
        emit logoutSuccess();
    }
   else if (command == "SELLER_HISTORY") {
    QString data = parts.mid(1).join("|");
    // FIX: Remove trailing |
    if (data.endsWith('|')) data.chop(1);
    emit sellerHistoryReceived(data);
}

else if (command == "ROOM_HISTORY") {
    QString data = parts.mid(1).join("|");
    // FIX: Remove trailing |
    if (data.endsWith('|')) data.chop(1);
    emit roomHistoryReceived(data);
}
    // Room responses
    else if (command == "CREATE_ROOM_SUCCESS") {
        // Server: CREATE_ROOM_SUCCESS|roomId|roomName
        // IMPORTANT: Server auto-joins creator to room!
        int roomId = parts.value(1).toInt();
        QString roomName = parts.value(2);
        emit roomCreated(roomId);
        // Emit joined since server auto-joins
        emit joinedRoom(roomId, roomName);
        // Auto refresh rooms
        sendListRooms();
    }
    else if (command == "CREATE_ROOM_FAIL") {
        emit roomError(parts.value(1, "Failed to create room"));
    }
    else if (command == "ROOM_LIST") {
        parseRoomListResponse(parts);
    }
    else if (command == "JOIN_ROOM_SUCCESS") {
        // Server: JOIN_ROOM_SUCCESS|roomId|roomName
        emit joinedRoom(parts.value(1).toInt(), parts.value(2));
    }
    else if (command == "JOIN_ROOM_FAIL") {
        emit roomError(parts.value(1, "Failed to join room"));
    }
    else if (command == "LEAVE_ROOM_SUCCESS") {
        emit leftRoom();
    }
    else if (command == "LEAVE_ROOM_FAIL") {
        emit roomError(parts.value(1, "Failed to leave room"));
    }
    
    // Auction responses
    else if (command == "CREATE_AUCTION_SUCCESS") {
        // Server: CREATE_AUCTION_SUCCESS|auctionId|title
        emit auctionCreated(parts.value(1).toInt());
        // Auto refresh auctions
        // Note: Need room_id to refresh, handled in UI
    }
    else if (command == "CREATE_AUCTION_FAIL") {
        emit auctionError(parts.value(1, "Failed to create auction"));
    }
    else if (command == "AUCTION_LIST") {
        parseAuctionListResponse(parts);
    }
    else if (command == "AUCTION_DETAILS") {
        // Server: AUCTION_DETAILS|id|title|desc|seller|startPrice|currentPrice|buyNowPrice|currentBidder|totalBids|timeLeft|status|roomName
        Auction auction;
        if (parts.size() >= 13) {
            auction.auctionId = parts[1].toInt();
            auction.title = parts[2];
            auction.description = parts[3];
            auction.sellerName = parts[4];
            auction.startPrice = parts[5].toDouble();
            auction.currentPrice = parts[6].toDouble();
            auction.buyNowPrice = parts[7].toDouble();
            auction.currentBidderName = parts[8];
            auction.totalBids = parts[9].toInt();
            int timeLeft = parts[10].toInt();
            auction.status = parts[11];
            
            if (timeLeft > 0) {
                auction.endTime = QDateTime::currentSecsSinceEpoch() + timeLeft;
            }
            
            emit auctionDetails(auction);
        }
    }
    else if (command == "DELETE_SUCCESS") {
        emit auctionDeleted();
    }
    else if (command == "DELETE_FAIL") {
        emit auctionError(parts.value(1, "Failed to delete"));
    }
    else if (command == "ACTIVATE_SUCCESS") {
        emit auctionActivated();
    }
    
    // Bidding responses
    else if (command == "BID_SUCCESS") {
        // Server: BID_SUCCESS|auctionId|amount|totalBids|timeLeft
        emit bidPlaced();
    }
    else if (command == "BID_FAIL") {
        // Server: BID_FAIL|errorCode|message
        emit bidError(parts.value(2, "Bid failed"));
    }
    else if (command == "BUY_NOW_SUCCESS") {
        emit buyNowSuccess();
    }
    else if (command == "BUY_NOW_FAIL") {
        emit bidError(parts.value(1, "Buy now failed"));
    }else if (command == "BID_HISTORY") {
    // Server: BID_HISTORY|bidId;username;amount;timestamp|...
    QString data = parts.mid(1).join("|");
    if (data.endsWith('|')) data.chop(1);
    emit bidHistoryReceived(data);
}
    
    // Account responses
    else if (command == "BALANCE_UPDATE") {
        emit balanceUpdated(parts.value(1).toDouble());
    }else if (command == "AUCTION_HISTORY") {
    // Server: AUCTION_HISTORY|auctionId;title;startPrice;finalPrice;winner;userBidCount;totalBids;participantCount;status|...
    QString data = parts.mid(1).join("|");
    // FIX: Remove trailing |
    if (data.endsWith('|')) data.chop(1);
    emit auctionHistoryReceived(data);
}    else if (command == "SEARCH_RESULTS") {
        qDebug() << "[NETWORK] ===== SEARCH_RESULTS RECEIVED =====";
        qDebug() << "[NETWORK] Command:" << command;
        qDebug() << "[NETWORK] Parts count:" << parts.size();
        
        QString data = parts.mid(1).join("|");
        if (data.endsWith('|')) data.chop(1);
        
        qDebug() << "[NETWORK] Data to emit:" << data;
        qDebug() << "[NETWORK] About to emit searchResultsReceived signal...";
        
        emit searchResultsReceived(data);
        
        qDebug() << "[NETWORK] Signal emitted!";
    }
    // Notifications - Real-time broadcasts
    else if (command.startsWith("NOTIF_")) {
        parseNotification(parts);
    }
    
    // Error
    else if (command == "ERROR") {
        qDebug() << "Server error:" << parts.value(1);
        emit connectionError(parts.value(1, "Unknown error"));
    }
}

void NetworkManager::parseLoginResponse(const QStringList& parts)
{
    // Server: LOGIN_SUCCESS|userId|username|balance
    if (parts.size() < 4) {
        emit loginFailed("Invalid response format");
        return;
    }
    
    User user;
    user.userId = parts[1].toInt();
    user.username = parts[2];
    user.balance = parts[3].toDouble();
    
    qDebug() << "Login successful:" << user.username << "Balance:" << user.balance;
    emit loginSuccess(user);
}

void NetworkManager::parseRoomListResponse(const QStringList& parts)
{
    QList<Room> rooms;
    
    // Server format: ROOM_LIST|roomId;name;creator;participants;maxPart|roomId2;...|
    // Example from handler.c: sprintf(temp, "%d;%s;%s;%d;%d|", roomId, name, creator, current, max);
    
    qDebug() << "parseRoomListResponse - total parts:" << parts.size();
    
    if (parts.size() < 2) {
        qDebug() << "Empty room list";
        emit roomListReceived(rooms);
        return;
    }
    
    // Skip first part (ROOM_LIST command), process rest
    for (int i = 1; i < parts.size(); i++) {
        QString roomData = parts[i];
        if (roomData.isEmpty()) continue;
        
        // Split by semicolon (from server format)
        QStringList fields = roomData.split(';');
        qDebug() << "Room" << i << "fields:" << fields;
        
        if (fields.size() >= 5) {
            Room room;
            room.roomId = fields[0].toInt();
            room.name = fields[1];
            room.creatorName = fields[2];
            room.currentParticipants = fields[3].toInt();
            room.maxParticipants = fields[4].toInt();
            room.totalAuctions = 0; // Server doesn't send this in list
            
            qDebug() << "Parsed room:" << room.roomId << room.name 
                     << room.currentParticipants << "/" << room.maxParticipants;
            rooms.append(room);
        } else {
            qDebug() << "Invalid room data, expected 5 fields, got:" << fields.size();
        }
    }
    
    qDebug() << "Total rooms parsed:" << rooms.size();
    emit roomListReceived(rooms);
}// OLD (WRONG):
// void NetworkManager::parseAuctionListResponse(const QString& data) {

// NEW (CORRECT):
void NetworkManager::parseAuctionListResponse(const QStringList& parts) {
    // Join parts back to string if needed
    QString data = parts.mid(1).join("|");  // Skip first part (command)
    
    QStringList auctionParts = data.split("|", Qt::SkipEmptyParts);
    QList<Auction> auctions;
    
    qDebug() << "parseAuctionListResponse - total parts:" << auctionParts.size();
    
    for (const QString& auctionStr : auctionParts) {
    QStringList fields = auctionStr.split(";");
    
    // Server format: id;title;currentPrice;buyNow;minInc;timeLeft;totalBids;status;sellerId;sellerName
    if (fields.size() >= 10) {
        Auction auction;
        auction.auctionId = fields.value(0).toInt();
        auction.title = fields.value(1);
        auction.currentPrice = fields.value(2).toDouble();
        auction.buyNowPrice = fields.value(3).toDouble();
        auction.minIncrement = fields.value(4).toDouble();
        
        int timeLeft = fields.value(5).toInt();
        auction.endTime = QDateTime::currentSecsSinceEpoch() + timeLeft;
        
        auction.totalBids = fields.value(6).toInt();
        auction.status = fields.value(7);
        auction.sellerId = fields.value(8).toInt();      // ← THÊM
        auction.sellerName = fields.value(9);            // ← THÊM
        
        qDebug() << "[PARSE] Auction" << auction.auctionId 
                 << "seller:" << auction.sellerName 
                 << "(id:" << auction.sellerId << ")";
        
        // Parse queue info if available (now at index 10+)
        if (fields.size() > 10) {
            auction.queuePosition = fields.value(10).toInt();
            auction.isQueued = (auction.queuePosition > 0);
        } else {
            auction.queuePosition = 0;
            auction.isQueued = false;
        }
        
        auctions.append(auction);
    }
}
    emit auctionListReceived(auctions);
}
void NetworkManager::sendSellerHistory(int userId)
{
    QString command = QString("SELLER_HISTORY%1%2")
        .arg(Constants::DELIMITER)
        .arg(userId);
    sendCommand(command);
}
void NetworkManager::parseNotification(const QStringList& parts)
{
    QString type = parts[0];
    QString message = parts.mid(1).join(Constants::DELIMITER);
    
    qDebug() << "Notification:" << type << message;
    emit notification(type, message);
    
    // Parse specific notification types
    if (type == "NOTIF_JOIN") {
    // Server: NOTIF_JOIN|userId|username
    QString username = parts.value(2);  // ← Index 2, không phải 1!
    emit userJoinedRoom(username);
}else if (type == "NOTIF_LEAVE") {
    // Server: NOTIF_LEAVE|userId|username
    QString username = parts.value(2);  // ← Index 2
    emit userLeftRoom(username);
}    else if (type == "NOTIF_AUCTION_NEW") {
        // New auction created: NOTIF_AUCTION_NEW|auctionId|title|creator|startPrice|buyNow|minInc|duration
        emit newAuction(parts.value(1).toInt(), parts.value(2));
    }else if (type == "NOTIF_BID") {
    // BEFORE: Server cũ gửi 7 fields
    // emit newBid(parts.value(1).toInt(), 
    //            parts.value(4).toDouble(),
    //            parts.value(3));
    
    // AFTER: Server mới gửi 5 fields
    // NOTIF_BID|auctionId|title|bidder|amount|timeLeft
    int auctionId = parts.value(1).toInt();
    QString title = parts.value(2);
    QString bidder = parts.value(3);    // ← USERNAME
    double amount = parts.value(4).toDouble();
    int timeLeft = parts.value(5).toInt();
    
    emit newBid(auctionId, amount, bidder);
}else if (type == "NOTIF_AUCTION_QUEUED") {
    emit notification(type, parts.mid(1).join("|"));
}
else if (type == "NOTIF_AUCTION_START") {
    emit notification(type, parts.mid(1).join("|"));
}
// ✅ THÊM: Handler cho NOTIF_AUCTION_STARTED (từ queue)
else if (type == "NOTIF_AUCTION_STARTED") {
    int auctionId = parts.value(1).toInt();
    QString title = parts.value(2);
    qDebug() << "[QUEUE] Auction started from queue:" << auctionId << title;
    
    emit notification(type, QString("🔨 Đấu giá bắt đầu: %1").arg(title));
    emit auctionStarted(auctionId);
}
else if (type == "NOTIF_QUEUE_EMPTY") {
    emit notification(type, "");
}   else if (type == "NOTIF_WARNING") {
        // Auction ending soon: NOTIF_WARNING|auctionId|secondsLeft
        emit auctionWarning(parts.value(1).toInt(), 
                           parts.value(2).toInt());
    }else if (type == "NOTIF_WINNER") {
    // NOTIF_WINNER|auctionId|title|winner|price|method
    int auctionId = parts.value(1).toInt();
    QString title = parts.value(2);
    QString winner = parts.value(3);  // ← Username
    double price = parts.value(4).toDouble();
    QString method = parts.value(5);
    
    emit auctionEnded(auctionId, winner, price);
}
    else if (type == "NOTIF_ROOM_NEW") {
        // New room broadcast: NOTIF_ROOM_NEW|roomId|roomName|creator
        qDebug() << "New room created:" << parts.value(2);
    }
    else if (type == "KICKED") {
        emit kicked(parts.value(1, "You have been kicked"));
    }
    else if (type == "NOTIF_AUCTION_DELETED") {
    // Broadcast khi auction bị xóa
    // Server: NOTIF_AUCTION_DELETED|auctionId|title
    int auctionId = parts.value(1).toInt();
    QString title = parts.value(2);
    emit auctionDeletedBroadcast(auctionId, title);
}
}