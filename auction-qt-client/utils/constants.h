#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    // Server configuration
    const QString DEFAULT_HOST = "127.0.0.1";
    const int DEFAULT_PORT = 8080;
    
    // Application info
    const QString APP_NAME = "Auction System";
    const QString APP_VERSION = "2.0";
    
    // UI Settings
    const int COUNTDOWN_UPDATE_INTERVAL = 1000; // ms
    const int AUTO_REFRESH_INTERVAL = 5000; // ms
    
    // Validation
    const double MIN_BID_INCREMENT = 10000.0;
    const int MIN_USERNAME_LENGTH = 3;
    const int MIN_PASSWORD_LENGTH = 6;
    const int MAX_USERNAME_LENGTH = 50;
    const int MAX_ROOM_NAME_LENGTH = 100;
    
    // Protocol delimiters
    const QString DELIMITER = "|";
    const QString LINE_END = "\n";
}

#endif // CONSTANTS_H
