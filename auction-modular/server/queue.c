#include "queue.h"
#include "database.h"
#include "broadcast.h"
#include "../shared/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

extern sqlite3 *g_db;

static int queue_processor_running = 0;
static pthread_t queue_processor_thread_id;

// Initialize queue for a room
int queue_init_room(int room_id, const char *mode)
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT OR REPLACE INTO room_queue_state "
                      "(room_id, queue_mode) VALUES (?, ?)";

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_text(stmt, 2, mode, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("[QUEUE] Initialized room %d with mode: %s\n", room_id, mode);

    return (result == SQLITE_DONE) ? 0 : -1;
}
int queue_add_auction(int room_id, int auction_id)
{
    sqlite3_stmt *stmt;

    // Get current max position in queue - ONLY QUEUED AUCTIONS
    const char *sql_max = "SELECT COALESCE(MAX(queue_position), 0) FROM auctions "
                          "WHERE room_id = ? AND status = 'queued'";

    sqlite3_prepare_v2(g_db, sql_max, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);

    int max_position = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        max_position = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // ✅ FIX: Position bắt đầu từ 1, không phải 0
    int new_position = max_position + 1;

    printf("[QUEUE] Adding auction %d to room %d at position %d\n",
           auction_id, room_id, new_position);

    const char *sql = "UPDATE auctions SET is_queued = 1, queue_position = ?, "
                      "status = 'queued' WHERE auction_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, new_position);
    sqlite3_bind_int(stmt, 2, auction_id);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_DONE)
    {
        printf("[QUEUE] ✅ Added auction %d to position %d\n", auction_id, new_position);
        return 0;
    }

    return -1;
}
int queue_get_delay(int room_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT auto_start_delay FROM room_queue_state WHERE room_id = ?";

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return 3; // Default 3 seconds
    }

    sqlite3_bind_int(stmt, 1, room_id);

    int delay = 3; // Default
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        delay = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // Ensure delay is positive
    if (delay <= 0)
        delay = 3;

    return delay;
}

// Remove auction from queue and reorder
int queue_remove_auction(int room_id, int auction_id)
{
    sqlite3_stmt *stmt;

    // Get position of auction to remove
    const char *sql_pos = "SELECT queue_position FROM auctions WHERE auction_id = ?";
    sqlite3_prepare_v2(g_db, sql_pos, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, auction_id);

    int removed_position = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        removed_position = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (removed_position == 0)
        return -1;

    sqlite3_exec(g_db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

    // Remove from queue
    const char *sql_remove = "UPDATE auctions SET is_queued = 0, queue_position = 0 WHERE auction_id = ?";
    sqlite3_prepare_v2(g_db, sql_remove, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, auction_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Reorder remaining auctions
    const char *sql_reorder = "UPDATE auctions SET queue_position = queue_position - 1 "
                              "WHERE room_id = ? AND is_queued = 1 AND queue_position > ?";
    sqlite3_prepare_v2(g_db, sql_reorder, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, removed_position);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_exec(g_db, "COMMIT;", NULL, NULL, NULL);

    printf("[QUEUE] Removed auction %d from room %d queue\n", auction_id, room_id);
    return 0;
}
int queue_get_next_auction(int room_id)
{
    sqlite3_stmt *stmt;

    // ✅ FIX: Lấy auction với position >= 1 (không phải = 0)
    const char *sql = "SELECT auction_id FROM auctions "
                      "WHERE room_id = ? "
                      "AND is_queued = 1 "
                      "AND status = 'queued' "
                      "AND queue_position >= 1 " // ← THÊM ĐIỀU KIỆN
                      "ORDER BY queue_position ASC "
                      "LIMIT 1";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);

    int next_auction_id = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        next_auction_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (next_auction_id > 0)
    {
        printf("[QUEUE] Next auction in room %d: %d\n", room_id, next_auction_id);
    }

    return next_auction_id;
}
int queue_get_current_auction(int room_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT current_auction_id FROM room_queue_state WHERE room_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);

    int auction_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auction_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return auction_id;
}
// Start next auction in queue
int queue_start_next_auction(int room_id)
{
    sqlite3_stmt *check_stmt;
    const char *check_sql = "SELECT auction_id FROM auctions "
                            "WHERE room_id = ? AND status = 'active'";

    sqlite3_prepare_v2(g_db, check_sql, -1, &check_stmt, NULL);
    sqlite3_bind_int(check_stmt, 1, room_id);

    if (sqlite3_step(check_stmt) == SQLITE_ROW)
    {
        int active_id = sqlite3_column_int(check_stmt, 0);
        sqlite3_finalize(check_stmt);

        printf("[QUEUE] ❌ Room %d already has active auction %d, cannot start next\n",
               room_id, active_id);
        return -1;
    }
    sqlite3_finalize(check_stmt);
    int next_auction_id = queue_get_next_auction(room_id);

    if (next_auction_id <= 0)
    {
        printf("[QUEUE] No more auctions in queue for room %d\n", room_id);
        return -1;
    }

    sqlite3_stmt *stmt;
    sqlite3_exec(g_db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

    // Activate the auction AND remove from queue
    time_t now = time(NULL);
    Auction auction;
    db_get_auction(next_auction_id, &auction);
    time_t end_time = now + auction.duration;

    // ✅ FIX: Thêm is_queued = 0, queue_position = 0
    const char *sql_activate = "UPDATE auctions SET "
                               "status = 'active', "
                               "is_queued = 0, "      // ← THÊM: Xóa khỏi queue
                               "queue_position = 0, " // ← THÊM: Reset position
                               "start_time = ?, "
                               "end_time = ? "
                               "WHERE auction_id = ?";

    sqlite3_prepare_v2(g_db, sql_activate, -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, now);
    sqlite3_bind_int64(stmt, 2, end_time);
    sqlite3_bind_int(stmt, 3, next_auction_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Update room queue state
    const char *sql_state = "UPDATE room_queue_state SET current_auction_id = ?, "
                            "next_auction_id = NULL WHERE room_id = ?";
    sqlite3_prepare_v2(g_db, sql_state, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, next_auction_id);
    sqlite3_bind_int(stmt, 2, room_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Get next in line (sau khi đã xóa auction hiện tại khỏi queue)
    int following_auction = queue_get_next_auction(room_id);
    if (following_auction > 0)
    {
        const char *sql_next = "UPDATE room_queue_state SET next_auction_id = ? WHERE room_id = ?";
        sqlite3_prepare_v2(g_db, sql_next, -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, following_auction);
        sqlite3_bind_int(stmt, 2, room_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_exec(g_db, "COMMIT;", NULL, NULL, NULL);

    // Broadcast auction started
    db_get_auction(next_auction_id, &auction);
    User seller;
    db_get_user(auction.seller_id, &seller);

    char broadcast_msg[BUFFER_SIZE * 2];
    sprintf(broadcast_msg, "NOTIF_AUCTION_STARTED|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
            next_auction_id, auction.title, seller.username,
            auction.start_price, auction.buy_now_price, auction.min_increment,
            auction.duration);
    broadcast_to_room(room_id, broadcast_msg, -1);

    printf("[QUEUE] Started auction %d (%s) in room %d\n",
           next_auction_id, auction.title, room_id);

    return 0; // ✅ Trả về 0 nếu thành công
}
void *queue_auto_processor_thread(void *arg)
{
    (void)arg;

    printf("[QUEUE] Auto-processor thread started\n");

    while (queue_processor_running)
    {
        sleep(30);

        sqlite3_stmt *stmt;
        const char *sql = "SELECT room_id, auto_start_delay FROM room_queue_state "
                          "WHERE queue_mode = 'auto'";

        if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            continue;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int room_id = sqlite3_column_int(stmt, 0);
            int delay = sqlite3_column_int(stmt, 1);

            // ✅ FIX: Kiểm tra có auction ACTIVE trong room không
            sqlite3_stmt *check_stmt;
            const char *check_sql = "SELECT COUNT(*) FROM auctions "
                                    "WHERE room_id = ? AND status = 'active'";

            int has_active = 0;
            if (sqlite3_prepare_v2(g_db, check_sql, -1, &check_stmt, NULL) == SQLITE_OK)
            {
                sqlite3_bind_int(check_stmt, 1, room_id);
                if (sqlite3_step(check_stmt) == SQLITE_ROW)
                {
                    has_active = sqlite3_column_int(check_stmt, 0);
                }
                sqlite3_finalize(check_stmt);
            }

            // ✅ CRITICAL: Chỉ start nếu KHÔNG CÓ auction active
            if (has_active > 0)
            {
                printf("[QUEUE] Room %d already has active auction, skipping\n", room_id);
                continue; // ← SKIP room này
            }

            // Tiếp tục logic cũ...
            int current_auction_id = queue_get_current_auction(room_id);

            if (current_auction_id <= 0)
            {
                // No current auction, check if queue has items
                int next_auction = queue_get_next_auction(room_id);
                if (next_auction > 0)
                {
                    printf("[QUEUE] Auto-starting first auction in room %d\n", room_id);
                    queue_start_next_auction(room_id);
                }
            }
            else
            {
                // Has current auction, check if it ended
                Auction current;
                if (db_get_auction(current_auction_id, &current) == 0)
                {
                    if (strcmp(current.status, "ended") == 0)
                    {
                        time_t now = time(NULL);
                        time_t end_plus_delay = current.end_time + delay;

                        if (now >= end_plus_delay)
                        {
                            printf("[QUEUE] Auto-starting next auction in room %d after delay\n", room_id);
                            int result = queue_start_next_auction(room_id);

                            if (result <= 0)
                            {
                                const char *sql_clear = "UPDATE room_queue_state "
                                                        "SET current_auction_id = NULL "
                                                        "WHERE room_id = ?";
                                sqlite3_stmt *clear_stmt;
                                if (sqlite3_prepare_v2(g_db, sql_clear, -1, &clear_stmt, NULL) == SQLITE_OK)
                                {
                                    sqlite3_bind_int(clear_stmt, 1, room_id);
                                    sqlite3_step(clear_stmt);
                                    sqlite3_finalize(clear_stmt);
                                }
                                printf("[QUEUE] Queue empty for room %d\n", room_id);
                            }
                        }
                    }
                }
            }
        }

        sqlite3_finalize(stmt);
    }

    printf("[QUEUE] Auto-processor thread stopped\n");
    return NULL;
}
void start_queue_processor()
{
    queue_processor_running = 1;

    if (pthread_create(&queue_processor_thread_id, NULL, queue_auto_processor_thread, NULL) != 0)
    {
        fprintf(stderr, "[ERROR] Failed to create queue processor thread\n");
        return;
    }

    pthread_detach(queue_processor_thread_id);
    printf("[QUEUE] Queue processor started\n");
}

void stop_queue_processor()
{
    queue_processor_running = 0;
}

// Query functions
int queue_is_auto_mode(int room_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT queue_mode FROM room_queue_state WHERE room_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);

    int is_auto = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *mode = (const char *)sqlite3_column_text(stmt, 0);
        is_auto = (strcmp(mode, "auto") == 0);
    }

    sqlite3_finalize(stmt);
    return is_auto;
}

int queue_get_position(int room_id, int auction_id)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT queue_position FROM auctions WHERE auction_id = ? AND room_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, auction_id);
    sqlite3_bind_int(stmt, 2, room_id);

    int position = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        position = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return position;
}

int queue_get_all_in_room(int room_id, Auction *auctions, int max_count)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM auctions WHERE room_id = ? AND is_queued = 1 "
                      "ORDER BY queue_position ASC LIMIT ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, max_count);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_count)
    {
        // Parse auction data (similar to db_get_auction)
        auctions[count].auction_id = sqlite3_column_int(stmt, 0);
        auctions[count].seller_id = sqlite3_column_int(stmt, 1);
        auctions[count].room_id = sqlite3_column_int(stmt, 2);
        // ... (copy rest of fields)
        count++;
    }

    sqlite3_finalize(stmt);
    return count;
}

int queue_set_mode(int room_id, const char *mode)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE room_queue_state SET queue_mode = ? WHERE room_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, mode, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, room_id);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("[QUEUE] Room %d queue mode set to: %s\n", room_id, mode);
    return (result == SQLITE_DONE) ? 0 : -1;
}

int queue_set_delay(int room_id, int delay_seconds)
{
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE room_queue_state SET auto_start_delay = ? WHERE room_id = ?";

    sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, delay_seconds);
    sqlite3_bind_int(stmt, 2, room_id);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE) ? 0 : -1;
}