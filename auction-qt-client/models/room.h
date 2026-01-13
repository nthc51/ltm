#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room {
public:
    int roomId;
    QString name;
    QString description;
    QString creatorName;
    int creatorId;
    int currentParticipants;
    int maxParticipants;
    int totalAuctions;
    qint64 createdAt;
    qint64 endTime;
    QString status;
    
    Room() 
        : roomId(0), creatorId(0), currentParticipants(0), 
          maxParticipants(10), totalAuctions(0), createdAt(0), 
          endTime(0), status("active") {}
    
    bool isFull() const {
        return currentParticipants >= maxParticipants;
    }
    
    bool isActive() const {
        return status == "active";
    }
    
    int availableSlots() const {
        return maxParticipants - currentParticipants;
    }
    
    QString getDisplayText() const {
        return QString("%1 (%2/%3 participants, %4 auctions)")
            .arg(name)
            .arg(currentParticipants)
            .arg(maxParticipants)
            .arg(totalAuctions);
    }
};

#endif // ROOM_H
