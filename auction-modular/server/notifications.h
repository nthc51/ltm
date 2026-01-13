#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "../shared/types.h"
#include "../shared/config.h"

// Notification types
#define NOTIF_TYPE_OUTBID "OUTBID"
#define NOTIF_TYPE_WON "WON"
#define NOTIF_TYPE_LOST "LOST"
#define NOTIF_TYPE_AUCTION_STARTED "AUCTION_STARTED"
#define NOTIF_TYPE_AUCTION_ENDED "AUCTION_ENDED"
#define NOTIF_TYPE_BID "BID"

typedef struct {
    int notif_id;
    int user_id;
    char notification_type[50];
    int auction_id;
    int room_id;
    char message[BUFFER_SIZE];
    time_t created_at;
    int is_read;
} PendingNotification;

// Notification queue management
int notif_save_pending(int user_id, const char *type, int auction_id, int room_id, const char *message);
int notif_get_pending(int user_id, PendingNotification *notifs, int max_count);
int notif_mark_read(int notif_id);
int notif_mark_all_read(int user_id);
int notif_delete(int notif_id);
int notif_count_unread(int user_id);

// Smart notification sending (tries real-time, falls back to queue)
int notif_send_to_user(int user_id, const char *type, int auction_id, int room_id, const char *message);

// Bulk send on reconnect
int notif_send_pending_to_user(int user_id, int socket);

// Cleanup old notifications
int notif_cleanup_old(int days_old);

#endif