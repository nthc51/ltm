#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    int userId;
    QString username;
    double balance;
    int currentRoomId;
    QString currentRoomName;
    
    User() 
        : userId(0), balance(0.0), currentRoomId(0) {}
    
    User(int id, const QString& name, double bal)
        : userId(id), username(name), balance(bal), currentRoomId(0) {}
    
    bool isValid() const {
        return userId > 0 && !username.isEmpty();
    }
    
    bool isInRoom() const {
        return currentRoomId > 0;
    }
    
    void clear() {
        userId = 0;
        username.clear();
        balance = 0.0;
        currentRoomId = 0;
        currentRoomName.clear();
    }
};

#endif // USER_H
