#ifndef FORMATTERS_H
#define FORMATTERS_H

#include <QString>
#include <QLocale>

class Formatters {
public:
    // Format currency (VND)
    static QString formatCurrency(double amount) {
        QLocale locale(QLocale::Vietnamese);
        return locale.toString(amount, 'f', 0) + " VND";
    }
    
    // Format time (seconds to MM:SS)
    static QString formatTime(int seconds) {
        if (seconds < 0) return "00:00";
        int mins = seconds / 60;
        int secs = seconds % 60;
        return QString("%1:%2")
            .arg(mins, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    }
    
    // Format date/time from timestamp
    static QString formatDateTime(qint64 timestamp) {
        QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
        return dt.toString("dd/MM/yyyy hh:mm:ss");
    }
    
    // Format time ago (e.g., "5 minutes ago")
    static QString formatTimeAgo(qint64 timestamp) {
        QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
        qint64 secondsAgo = dt.secsTo(QDateTime::currentDateTime());
        
        if (secondsAgo < 60) return QString("%1 seconds ago").arg(secondsAgo);
        if (secondsAgo < 3600) return QString("%1 minutes ago").arg(secondsAgo / 60);
        if (secondsAgo < 86400) return QString("%1 hours ago").arg(secondsAgo / 3600);
        return QString("%1 days ago").arg(secondsAgo / 86400);
    }
};

#endif // FORMATTERS_H
