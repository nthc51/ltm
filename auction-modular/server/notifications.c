#include "notifications.h"
#include "database.h"
#include "network.h"
#include "../shared/config.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

extern sqlite3 *g_db;
extern ClientSession g_clients[];
extern pthread_mutex_t g_client_mutex;

// Save notification for offline user
int notif_save_pending(int user_id, const char *type, int auction_id, int room_id, const char *message) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO pending_notifications "
                     "(user_id, notification_type, auction_id, room_id, message) "
                     "VALUES (?, ?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, auction_id);
    sqlite3_bind_int(stmt, 4, room_id);
    sqlite3_bind_text(stmt, 5, message, -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result == SQLITE_DONE) {
        printf("[NOTIF] Queued notification for offline user %d (type: %s)\n", user_id, type);
        return 0;
    }
    
    return -1;
}

// Get pending notifications for user
int notif_get_pending(int user_id, PendingNotification *notifs, int max_count) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT notif_id, user_id, notification_type, auction_id, "
                     "room_id, message, created_at, is_read "
                     "FROM pending_notifications "
                     "WHERE user_id = ? AND is_read = 0 "
                     "ORDER BY created_at ASC LIMIT ?";
    
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, max_count);
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_count) {
        notifs[count].notif_id = sqlite3_column_int(stmt, 0);
        notifs[count].user_id = sqlite3_column_int(stmt, 1);
        strncpy(notifs[count].notification_type, 
                (const char*)sqlite3_column_text(stmt, 2), 
                sizeof(notifs[count].notification_type) - 1);
        notifs[count].auction_id = sqlite3_column_int(stmt, 3);
        notifs[count].room_id = sqlite3_column_int(stmt, 4);
        strncpy(notifs[count].message, 
                (const char*)sqlite3_column_text(stmt, 5), 
                sizeof(notifs[count].message) - 1);
        notifs[count].created_at = sqlite3_column_int64(stmt, 6);
        notifs[count].is_read = sqlite3_column_int(stmt, 7);
        count++;
    }
    
    sqlite3_finalize(stmt);
    return count;
}

// Mark notification as read
int notif_mark_read(int notif_id) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE pending_notifications SET is_read = 1 WHERE notif_id = ?";
    
    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, notif_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return (result == SQLITE_DONE) ? 0 : -1;
}

// Mark all notifications as read for user
int notif_mark_all_read(int user_id) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE pending_notifications SET is_read = 1 WHERE user_id = ?";
    
    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return (result == SQLITE_DONE) ? 0 : -1;
}

// Count unread notifications
int notif_count_unread(int user_id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM pending_notifications "
                     "WHERE user_id = ? AND is_read = 0";
    
    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return count;
}

// Check if user is online
static int is_user_online(int user_id) {
    pthread_mutex_lock(&g_client_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_clients[i].is_active && g_clients[i].user_id == user_id) {
            pthread_mutex_unlock(&g_client_mutex);
            return 1;
        }
    }
    
    pthread_mutex_unlock(&g_client_mutex);
    return 0;
}

static int get_user_socket(int user_id) {
    pthread_mutex_lock(&g_client_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_clients[i].is_active && g_clients[i].user_id == user_id) {
            int socket = g_clients[i].socket;
            pthread_mutex_unlock(&g_client_mutex);
            return socket;
        }
    }
    
    pthread_mutex_unlock(&g_client_mutex);
    return -1;
}

// Smart send: Try real-time, fallback to queue
int notif_send_to_user(int user_id, const char *type, int auction_id, int room_id, const char *message) {
    if (is_user_online(user_id)) {
        // User is online, send directly
        int socket = get_user_socket(user_id);
        if (socket >= 0) {
            if (send(socket, message, strlen(message), 0) > 0) {
                printf("[NOTIF] Sent real-time to user %d: %s\n", user_id, type);
                return 1;  // Sent successfully
            }
        }
    }
    
    // User offline or send failed, queue it
    notif_save_pending(user_id, type, auction_id, room_id, message);
    return 0;  // Queued for later
}

// Send all pending notifications on reconnect
int notif_send_pending_to_user(int user_id, int socket) {
    PendingNotification notifs[50];
    int count = notif_get_pending(user_id, notifs, 50);
    
    if (count == 0) {
        return 0;
    }
    
    printf("[NOTIF] Sending %d pending notifications to user %d\n", count, user_id);
    
    // Send info message first
    char info_msg[BUFFER_SIZE];
    sprintf(info_msg, "NOTIF_INFO|You have %d unread notifications\n", count);
    send(socket, info_msg, strlen(info_msg), 0);
    
    // Send each notification
    int sent = 0;
    for (int i = 0; i < count; i++) {
        if (send(socket, notifs[i].message, strlen(notifs[i].message), 0) > 0) {
            notif_mark_read(notifs[i].notif_id);
            sent++;
        }
    }
    
    printf("[NOTIF] Successfully sent %d/%d notifications to user %d\n", sent, count, user_id);
    return sent;
}

// Cleanup old read notifications
int notif_cleanup_old(int days_old) {
    sqlite3_stmt *stmt;
    time_t cutoff = time(NULL) - (days_old * 24 * 60 * 60);
    
    const char *sql = "DELETE FROM pending_notifications "
                     "WHERE is_read = 1 AND created_at < ?";
    
    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, cutoff);
    
    sqlite3_step(stmt);
    int deleted = sqlite3_changes(g_db);
    sqlite3_finalize(stmt);
    
    if (deleted > 0) {
        printf("[NOTIF] Cleaned up %d old notifications\n", deleted);
    }
    
    return deleted;
}