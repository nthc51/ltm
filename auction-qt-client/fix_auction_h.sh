#!/bin/bash

# Fix auction.h - Add missing include

cat > models/auction.h << 'AUCTIONH'
#ifndef AUCTION_H
#define AUCTION_H

#include <QString>
#include <QDateTime>

class Auction {
public:
    int auctionId;
    int sellerId;
    QString sellerName;
    int roomId;
    QString title;
    QString description;
    double startPrice;
    double currentPrice;
    double buyNowPrice;
    double minIncrement;
    int currentBidderId;
    QString currentBidderName;
    qint64 startTime;
    qint64 endTime;
    int duration;
    int totalBids;
    QString status;
    int winnerId;
    QString winnerName;
    QString winMethod;
    
    Auction() 
        : auctionId(0), sellerId(0), roomId(0), startPrice(0.0), 
          currentPrice(0.0), buyNowPrice(0.0), minIncrement(10000.0),
          currentBidderId(0), startTime(0), endTime(0), duration(0), 
          totalBids(0), status("waiting"), winnerId(0) {}
    
    bool isActive() const {
        return status == "active";
    }
    
    bool isWaiting() const {
        return status == "waiting";
    }
    
    bool isEnded() const {
        return status == "ended";
    }
    
    bool canBid() const {
        return isActive();
    }
    
    bool hasBuyNow() const {
        return buyNowPrice > 0;
    }
    
    int getTimeLeft() const {
        if (!isActive()) return 0;
        qint64 now = QDateTime::currentSecsSinceEpoch();
        qint64 left = endTime - now;
        return left > 0 ? static_cast<int>(left) : 0;
    }
    
    double getMinimumBid() const {
        return currentPrice + minIncrement;
    }
    
    QString getStatusText() const {
        if (isWaiting()) return "Waiting";
        if (isActive()) return "Active";
        if (isEnded()) return "Ended";
        return status;
    }
};

#endif // AUCTION_H
AUCTIONH

echo "✅ Fixed auction.h - Added QDateTime include"
