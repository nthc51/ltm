#ifndef AUCTION_H
#define AUCTION_H

#include <QString>
#include <QDateTime>

class Auction
{
public:
    int auctionId;
    int sellerId;
    int roomId;
    QString title;
    QString description;
    QString sellerName;
    double startPrice;
    double currentPrice;
    double buyNowPrice;
    double minIncrement;
    int currentBidderId;
    QString currentBidderName;
    int totalBids;
    qint64 endTime;
    QString status;
    int queuePosition;        // ← THÊM: vị trí trong hàng đợi
    bool isQueued;            // ← THÊM: có đang trong queue không
    
    Auction() 
        : auctionId(0), 
          sellerId(0), 
          roomId(0),
          startPrice(0.0), 
          currentPrice(0.0), 
          buyNowPrice(0.0),
          minIncrement(1000.0),
          currentBidderId(0), 
          totalBids(0), 
          endTime(0),
          queuePosition(0),   // ← THÊM: init = 0
          isQueued(false)     // ← THÊM: init = false
    {}
    
    bool isActive() const { 
        return status == "active"; 
    }
    
    bool isWaiting() const { 
        return status == "waiting"; 
    }
    
    bool isEnded() const { 
        return status == "ended"; 
    }
    
    // ← THÊM: Check nếu đang trong queue
    bool isQueuedStatus() const {
        return status == "queued";
    }
    
    bool canBid() const { 
        return isActive() && getTimeLeft() > 0; 
    }
    
    bool hasBuyNow() const { 
        return buyNowPrice > 0; 
    }
    
    int getTimeLeft() const {
        if (!isActive() || endTime == 0) return 0;
        qint64 now = QDateTime::currentSecsSinceEpoch();
        int left = endTime - now;
        return (left > 0) ? left : 0;
    }
    
    QString getStatusText() const {
        if (isActive()) return "Đang đấu giá";
        if (isWaiting()) return "Chờ kích hoạt";
        if (isQueuedStatus()) return QString("Hàng đợi #%1").arg(queuePosition);  // ← THÊM
        if (isEnded()) return "Đã kết thúc";
        return "Không xác định";
    }
};

#endif // AUCTION_H