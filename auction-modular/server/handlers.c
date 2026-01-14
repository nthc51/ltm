
#include "handlers.h"
#include "database.h"
#include "network.h"
#include "broadcast.h"
#include "../shared/config.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <stdlib.h> 
#include "queue.h"
#include <unistd.h>

void handle_register(int client_socket, char *data) {
    char username[50], password[100];
    double balance = 50000000.0;
    
    sscanf(data, "%[^|]|%s", username, password);
    
    int user_id = db_create_user(username, password, balance);
    
    char response[BUFFER_SIZE];
    if (user_id > 0) {
        sprintf(response, "REGISTER_SUCCESS|%d|%s\n", user_id, username);
        printf("[INFO] New user registered: %s (ID: %d)\n", username, user_id);
    } else {
        sprintf(response, "REGISTER_FAIL|Username already exists\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}

void handle_login(int client_socket, char *data) {
    char username[50], password[100];
    sscanf(data, "%[^|]|%s", username, password);
    
    int user_id = db_authenticate_user(username, password);
    
    char response[BUFFER_SIZE];
    if (user_id > 0) {
        User user;
        db_get_user(user_id, &user);
        
        kick_existing_session(user_id);
        
        sprintf(response, "LOGIN_SUCCESS|%d|%s|%.2f\n", 
                user_id, username, user.balance);
        add_client_session(client_socket, user_id);
        printf("[INFO] User logged in: %s (ID: %d)\n", username, user_id);
    } else {
        sprintf(response, "LOGIN_FAIL|Invalid username or password\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}

void handle_create_room(int client_socket, char *data) {
    int creator_id, max_participants, duration;
    char name[100], desc[200];
    
    sscanf(data, "%d|%[^|]|%[^|]|%d|%d",
           &creator_id, name, desc, &max_participants, &duration);
    
    int current_room = db_get_user_room(creator_id);
    if (current_room > 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "CREATE_ROOM_FAIL|You must leave your current room first\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    int room_id = db_create_room(creator_id, name, desc, max_participants, duration);
    
    char response[BUFFER_SIZE];
    if (room_id > 0) {
        db_join_room(creator_id, room_id);
        update_client_room(creator_id, room_id);
        
        // BROADCAST NEW ROOM
        User creator;
        if (db_get_user(creator_id, &creator) == 0) {
            broadcast_room_created(name, creator.username, client_socket);
        }
        
        sprintf(response, "CREATE_ROOM_SUCCESS|%d|%s\n", room_id, name);
        printf("[INFO] Room created: %s (ID: %d)\n", name, room_id);
    } else {
        sprintf(response, "CREATE_ROOM_FAIL|Failed to create room\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}

void handle_list_rooms(int client_socket) {
    AuctionRoom rooms[MAX_ROOMS];
    int count = db_get_all_rooms(rooms, MAX_ROOMS);
    
    char response[BUFFER_SIZE * 4] = "ROOM_LIST|";
    char temp[256];
    
    for (int i = 0; i < count; i++) {
        User creator;
        db_get_user(rooms[i].created_by, &creator);
        
        sprintf(temp, "%d;%s;%s;%d;%d|",
                rooms[i].room_id,
                rooms[i].room_name,
                creator.username,
                rooms[i].current_participants,
                rooms[i].max_participants);
        strcat(response, temp);
    }
    
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
}void handle_seller_history(int client_socket, char *data) {
    int user_id;
    sscanf(data, "%d", &user_id);
    
    sqlite3_stmt *stmt;
    // FIX: Đơn giản hóa query
    const char *sql = 
    "SELECT a.auction_id, a.title, a.start_price, a.current_price, "
    "  COALESCE((SELECT u.username FROM users u WHERE u.user_id =  a.winner_id), 'No winner') as winner, "
    "  a.total_bids, "
    "  (SELECT COUNT(DISTINCT user_id) FROM bids b WHERE b.auction_id = a.auction_id) as participant_count, "
    "  a.status, "
    "  COALESCE(a.win_method, '-') as win_method "  // ← THÊM DÒNG NÀY
    "FROM auctions a "
    "WHERE a.seller_id = ? "
    "ORDER BY a.end_time DESC";
    
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("[ERROR] SQL prepare failed: %s\n", sqlite3_errmsg(g_db));
        char response[BUFFER_SIZE];
        sprintf(response, "ERROR|Failed to get seller history\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    char response[BUFFER_SIZE * 4] = "SELLER_HISTORY|";
    char temp[256];
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
    int auction_id = sqlite3_column_int(stmt, 0);
    const char *title = (const char*)sqlite3_column_text(stmt, 1);
    double start_price = sqlite3_column_double(stmt, 2);
    double final_price = sqlite3_column_double(stmt, 3);
    const char *winner = (const char*)sqlite3_column_text(stmt, 4);
    int total_bids = sqlite3_column_int(stmt, 5);
    int participants = sqlite3_column_int(stmt, 6);
    const char *status = (const char*)sqlite3_column_text(stmt, 7);
    const char *win_method = (const char*)sqlite3_column_text(stmt, 8);  // ← THÊM DÒNG NÀY
    
    // Format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status;winMethod
    sprintf(temp, "%d;%s;%.2f;%.2f;%s;%d;%d;%s;%s|",  // ← Thêm %s
            auction_id, title, start_price, final_price, winner, 
            total_bids, participants, status, win_method);  // ← Thêm win_method
    strcat(response, temp);
    count++;
}
    
    sqlite3_finalize(stmt);
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
    
    printf("[INFO] Sent seller history to user %d: %d auctions\n", user_id, count);
}
void handle_join_room(int client_socket, char *data) {
    int user_id, room_id;
    sscanf(data, "%d|%d", &user_id, &room_id);
    
    int result = db_join_room(user_id, room_id);
    
    char response[BUFFER_SIZE];
    if (result == 0) {
        AuctionRoom room;
        db_get_room(room_id, &room);
        
        // UPDATE CLIENT ROOM FIRST!
        update_client_room(user_id, room_id);
        
        // THEN BROADCAST JOIN
        User user;
        if (db_get_user(user_id, &user) == 0) {
            broadcast_join_room(room_id, user.username, client_socket);
        }
        
        sprintf(response, "JOIN_ROOM_SUCCESS|%d|%s\n", room_id, room.room_name);
        printf("[INFO] User %d joined room %d\n", user_id, room_id);
    } else {
        const char *error_msg;
        switch(result) {
            case -1: error_msg = "Room not found"; break;
            case -3: error_msg = "Room is full"; break;
            case -4: error_msg = "Already in another room"; break;
            default: error_msg = "Unknown error"; break;
        }
        sprintf(response, "JOIN_ROOM_FAIL|%s\n", error_msg);
    }
    
    send(client_socket, response, strlen(response), 0);
}

void handle_leave_room(int client_socket, char *data) {
    int user_id;
    sscanf(data, "%d", &user_id);
    
    // Get room before leaving
    int old_room = db_get_user_room(user_id);
    
    int result = db_leave_room(user_id);
    
    char response[BUFFER_SIZE];
    if (result == 0) {
        // BROADCAST LEAVE
        User user;
        if (old_room > 0 && db_get_user(user_id, &user) == 0) {
            broadcast_leave_room(old_room, user.username, client_socket);
        }
        
        sprintf(response, "LEAVE_ROOM_SUCCESS|\n");
        update_client_room(user_id, -1);
        printf("[INFO] User %d left room\n", user_id);
    } else {
        sprintf(response, "LEAVE_ROOM_FAIL|Not in any room\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}
void handle_create_auction(int client_socket, char *data) {
   
    int seller_id, room_id, duration;
    double start_price, buy_now_price, min_increment;
    char title[256], description[1024];
    
    sscanf(data, "%d|%d|%[^|]|%[^|]|%lf|%lf|%lf|%d",
           &seller_id, &room_id, title, description,
           &start_price, &buy_now_price, &min_increment, &duration);
    // ✅ FIX: Chỉ chủ phòng mới được tạo auction
AuctionRoom room;
if (db_get_room(room_id, &room) != 0) {
    char response[] = "AUCTION_ERROR|Room not found\n";
    send(client_socket, response, strlen(response), 0);
    return;
}
if (room.created_by != seller_id) {
    printf("[ERROR] User %d is not owner of room %d (owner: %d)\n", 
           seller_id, room_id, room.created_by);
    char response[] = "AUCTION_ERROR|Only room owner can create auctions\n";
    send(client_socket, response, strlen(response), 0);
    return;
}
    printf("[DEBUG] Create auction: seller=%d, room=%d, title='%s', duration=%d\n",
           seller_id, room_id, title, duration);
    
    // Create auction (status = 'waiting' initially)
    int auction_id = db_create_auction(seller_id, room_id, title, description,
                                       start_price, buy_now_price, min_increment, duration);
    
    if (auction_id > 0) {
        printf("[AUCTION] Created auction %d in room %d\n", auction_id, room_id);
        
        // Get seller username
        User seller;
        char seller_username[50] = "Unknown";
        if (db_get_user(seller_id, &seller) == 0) {
            strcpy(seller_username, seller.username);
        }
        
        // ═══════════════════════════════════════════════════════
        // CRITICAL: Get ACTIVE auction in room (not just current in queue state)
        // ═══════════════════════════════════════════════════════
        
        // Check for ANY active auction in this room
        sqlite3_stmt *stmt;
        const char *sql_check = "SELECT auction_id FROM auctions "
                               "WHERE room_id = ? AND status = 'active' LIMIT 1";
        int has_active = 0;
        
        if (sqlite3_prepare_v2(g_db, sql_check, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, room_id);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                has_active = sqlite3_column_int(stmt, 0);
                printf("[DEBUG] Room %d has active auction: %d\n", room_id, has_active);
            }
            sqlite3_finalize(stmt);
        }
        
        if (has_active > 0) {
            // ═══════════════════════════════════════════════════════
            // ROOM HAS ACTIVE AUCTION → ADD TO QUEUE
            // ═══════════════════════════════════════════════════════
            int result = queue_add_auction(room_id, auction_id);
            
            if (result == 0) {
                int position = queue_get_position(room_id, auction_id);
                
                printf("[QUEUE] Auction %d added to queue at position %d\n", 
                       auction_id, position);
                
                char response[BUFFER_SIZE];
                sprintf(response, "AUCTION_CREATED|%d\n", auction_id);
                send(client_socket, response, strlen(response), 0);
                
                char notif[BUFFER_SIZE];
                sprintf(notif, "NOTIF_AUCTION_QUEUED|%d|%s|%s|%d\n",
                        auction_id, title, seller_username, position);
                broadcast_to_room(room_id, notif, -1);
                
                printf("[BROADCAST] Sent queue notification to room %d\n", room_id);
            } else {
                // Failed to add to queue - activate it anyway
                printf("[WARN] Failed to add to queue, activating immediately\n");
                db_activate_auction(auction_id, seller_id);
                
                char response[BUFFER_SIZE];
                sprintf(response, "AUCTION_CREATED|%d\n", auction_id);
                send(client_socket, response, strlen(response), 0);
                
                char notif[BUFFER_SIZE];
                sprintf(notif, "NOTIF_AUCTION_NEW|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
                        auction_id, title, seller_username, start_price, 
                        buy_now_price, min_increment, duration);
                broadcast_to_room(room_id, notif, -1);
            }
            
        } else {
            // ═══════════════════════════════════════════════════════
            // NO ACTIVE AUCTION → START IMMEDIATELY
            // ═══════════════════════════════════════════════════════
            printf("[QUEUE] No active auction in room %d, starting auction %d immediately\n", 
                   room_id, auction_id);
            
            db_activate_auction(auction_id, seller_id);
            
            // Initialize queue state if needed
            queue_init_room(room_id, "auto");
            
            // Set current auction
            const char *sql_update = "UPDATE room_queue_state SET current_auction_id = ? "
                                    "WHERE room_id = ?";
            if (sqlite3_prepare_v2(g_db, sql_update, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, auction_id);
                sqlite3_bind_int(stmt, 2, room_id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            char response[BUFFER_SIZE];
            sprintf(response, "AUCTION_CREATED|%d\n", auction_id);
            send(client_socket, response, strlen(response), 0);
            
            char notif[BUFFER_SIZE];
            sprintf(notif, "NOTIF_AUCTION_NEW|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
                    auction_id, title, seller_username, start_price, 
                    buy_now_price, min_increment, duration);
            broadcast_to_room(room_id, notif, -1);
            
            printf("[BROADCAST] New auction started in room %d\n", room_id);
        }
        
    } else {
        char response[BUFFER_SIZE];
        sprintf(response, "AUCTION_CREATE_FAIL|Failed to create auction\n");
        send(client_socket, response, strlen(response), 0);
    }
}
void handle_set_queue_mode(int client_socket, char *data) {
    int room_id;
    char mode[20];
    
    sscanf(data, "%d|%s", &room_id, mode);
    
    if (strcmp(mode, "auto") != 0 && strcmp(mode, "manual") != 0) {
        char response[] = "QUEUE_MODE_FAIL|Invalid mode\n";
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    int result = queue_set_mode(room_id, mode);
    
    if (result == 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "QUEUE_MODE_SUCCESS|%s\n", mode);
        send(client_socket, response, strlen(response), 0);
        
        printf("[QUEUE] Room %d queue mode set to: %s\n", room_id, mode);
    } else {
        char response[] = "QUEUE_MODE_FAIL|Failed to set mode\n";
        send(client_socket, response, strlen(response), 0);
    }
}void handle_list_auctions(int client_socket, char *data) {
    int room_id;
    sscanf(data, "%d", &room_id);
    
    Auction auctions[MAX_AUCTIONS];
    int count = db_get_auctions_by_room(room_id, auctions, MAX_AUCTIONS);
    
    char response[BUFFER_SIZE * 4] = "AUCTION_LIST|";
    char temp[512];
    time_t now = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int time_left = 0;
        if (strcmp(auctions[i].status, "active") == 0) {
            time_left = auctions[i].end_time - now;
            if (time_left < 0) time_left = 0;
        }
        
        // ✅ Get seller username
        User seller;
        char seller_name[50] = "Unknown";
        if (db_get_user(auctions[i].seller_id, &seller) == 0) {
            strncpy(seller_name, seller.username, sizeof(seller_name) - 1);
            seller_name[sizeof(seller_name) - 1] = '\0';  // Ensure null termination
        }
        
        // ✅ Format: id;title;currentPrice;buyNow;minInc;timeLeft;totalBids;status;sellerId;sellerName
        sprintf(temp, "%d;%s;%.2f;%.2f;%.2f;%d;%d;%s;%d;%s|",
            auctions[i].auction_id,
            auctions[i].title,
            auctions[i].current_price,
            auctions[i].buy_now_price,
            auctions[i].min_increment,
            time_left,
            auctions[i].total_bids,
            auctions[i].status,
            auctions[i].seller_id,      // ← THÊM
            seller_name);                // ← THÊM
        
        strcat(response, temp);
    }
    
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
}
void handle_view_auction(int client_socket, char *data) {
    int auction_id;
    sscanf(data, "%d", &auction_id);
    
    Auction auction;
    char seller_name[50] = {0};
    char room_name[100] = {0};
    
    if (db_get_full_auction_details(auction_id, &auction, seller_name, room_name) == 0) {
        time_t now = time(NULL);
        int time_left = (auction.end_time > now) ? (auction.end_time - now) : 0;
        
        char current_bidder[50] = "None";
        if (auction.current_bidder_id > 0) {
            User bidder;
            if (db_get_user(auction.current_bidder_id, &bidder) == 0) {
                strncpy(current_bidder, bidder.username, sizeof(current_bidder)-1);
            }
        }
        
        char response[BUFFER_SIZE * 2];
        sprintf(response, "AUCTION_DETAILS|%d|%s|%s|%s|%.2f|%.2f|%.2f|%s|%d|%d|%s|%s\n",
                auction_id, auction.title, auction.description, seller_name,
                auction.start_price, auction.current_price, auction.buy_now_price,
                current_bidder, auction.total_bids, time_left, auction.status, room_name);
        
        send(client_socket, response, strlen(response), 0);
    } else {
        char response[BUFFER_SIZE];
        sprintf(response, "AUCTION_DETAILS_FAIL|Auction not found\n");
        send(client_socket, response, strlen(response), 0);
    }
}
void handle_search_auctions(int client_socket, char *data) {
    SearchFilter filter;
    memset(&filter, 0, sizeof(SearchFilter));
    
    // Initialize default values
    filter.min_price = -1;
    filter.max_price = -1;
    filter.min_time_left = -1;
    filter.max_time_left = -1;
    filter.room_id = -1;
    filter.keyword[0] = '\0';
    filter.status[0] = '\0';
    
    // Manual parsing for format: room_id|keyword|min_price|max_price
    // Cannot use strtok because it skips empty tokens (||)
    
    char *ptr = data;
    char *field_start;
    int field_num = 0;
    
    // Parse each field separated by |
    while (*ptr != '\0' && field_num < 4) {
        field_start = ptr;
        
        // Find next delimiter or end of string
        while (*ptr != '|' && *ptr != '\0') {
            ptr++;
        }
        
        // Extract field value
        int field_len = ptr - field_start;
        char field_value[256] = {0};
        if (field_len > 0) {
            strncpy(field_value, field_start, field_len);
            field_value[field_len] = '\0';
        }
        
        // Parse based on field number
        switch (field_num) {
            case 0: // room_id
                if (field_len > 0) {
                    filter.room_id = atoi(field_value);
                }
                break;
            case 1: // keyword (can be empty)
                if (field_len > 0) {
                    strncpy(filter.keyword, field_value, sizeof(filter.keyword) - 1);
                }
                break;
            case 2: // min_price
                if (field_len > 0) {
                    filter.min_price = atof(field_value);
                }
                break;
            case 3: // max_price
                if (field_len > 0) {
                    filter.max_price = atof(field_value);
                }
                break;
        }
        
        // Move to next field
        if (*ptr == '|') {
            ptr++;
        }
        field_num++;
    }
    
    // DEBUG LOGS
    printf("[DEBUG] ===== SEARCH REQUEST =====\n");
    printf("[DEBUG] Raw data: '%s'\n", data);
    printf("[DEBUG] Parsed values:\n");
    printf("[DEBUG]   Room ID: %d\n", filter.room_id);
    printf("[DEBUG]   Keyword: '%s' (len=%lu)\n", filter.keyword, strlen(filter.keyword));
    printf("[DEBUG]   Min price: %.2f\n", filter.min_price);
    printf("[DEBUG]   Max price: %.2f\n", filter.max_price);
    printf("[DEBUG] Filter conditions:\n");
    printf("[DEBUG]   min_price >= 0? %s\n", filter.min_price >= 0 ? "YES" : "NO");
    printf("[DEBUG]   max_price >= 0? %s\n", filter.max_price >= 0 ? "YES" : "NO");
    printf("[DEBUG] =============================\n");
    
    // Set default status to search only active auctions
    strcpy(filter.status, "active");
    
    // Search database
    Auction results[MAX_AUCTIONS];
    int count = db_search_auctions(filter, results, MAX_AUCTIONS);
    
    // Build response (rest of code unchanged)
    char response[BUFFER_SIZE * 4] = "SEARCH_RESULTS|";
    char temp[512];
    time_t now = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int time_left = 0;
        if (strcmp(results[i].status, "active") == 0) {
            time_left = results[i].end_time - now;
            if (time_left < 0) time_left = 0;
        }
        
        User seller;
        if (db_get_user(results[i].seller_id, &seller) != 0) {
            strcpy(seller.username, "Unknown");
        }
        
        AuctionRoom room;
        if (db_get_room(results[i].room_id, &room) != 0) {
            strcpy(room.room_name, "Unknown");
        }
        
        sprintf(temp, "%d;%s;%.2f;%.2f;%d;%d;%s;%s;%s|",
                results[i].auction_id, results[i].title,
                results[i].current_price, results[i].buy_now_price,
                time_left, results[i].total_bids, results[i].status,
                seller.username, room.room_name);
        strcat(response, temp);
    }
    
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
    
    printf("[INFO] Search completed: %d results for user in room %d\n", 
           count, filter.room_id);
}void handle_place_bid(int client_socket, char *data) {
    int auction_id, user_id;
    double bid_amount;
    
    sscanf(data, "%d|%d|%lf", &auction_id, &user_id, &bid_amount);
    
    int seller_id = 0, room_id = 0;
    if (db_get_auction_seller_and_room(auction_id, &seller_id, &room_id) != 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-1|Auction not found\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    if (seller_id == user_id) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-5|You cannot bid on your own auction\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    Auction auction;
    if (db_get_auction_with_room(auction_id, &auction, &room_id) != 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-1|Auction not found\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    if (strcmp(auction.status, "active") != 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-2|Auction is not active\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    time_t now = time(NULL);
    int time_left = auction.end_time - now;
    if (time_left <= 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-2|Auction has ended\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // ==================== FIX: CHECK MIN INCREMENT ====================
    double min_required = auction.current_price + auction.min_increment;
    if (bid_amount < min_required) {
        char response[BUFFER_SIZE];
        sprintf(response, "BID_FAIL|-3|Bid too low. Minimum: %.2f (Current: %.2f + Step: %.2f)\n",
                min_required, auction.current_price, auction.min_increment);
        send(client_socket, response, strlen(response), 0);
        printf("[BID] Rejected: %.2f < %.2f (min required)\n", bid_amount, min_required);
        return;
    }
    // ==================================================================
    
    int result = db_place_bid(auction_id, user_id, bid_amount);
    
    char response[BUFFER_SIZE];
    if (result == 0) {
        db_get_auction_with_room(auction_id, &auction, &room_id);
        
        // ANTI-SNIPE: If bid with < 30s left, extend to 30s
        time_left = auction.end_time - now;
        if (time_left < 30 && time_left > 0) {
            int extend_by = 30 - time_left;
            sqlite3_stmt *stmt;
            const char *sql = "UPDATE auctions SET end_time = end_time + ? WHERE auction_id = ?";
            if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, extend_by);
                sqlite3_bind_int(stmt, 2, auction_id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                printf("[ANTI-SNIPE] Extended auction %d by %ds\n", auction_id, extend_by);
            }
            time_left = 30;
        }
        
        // BROADCAST TO ALL (including bidder)
        User bidder;
        if (db_get_user(user_id, &bidder) == 0) {
            broadcast_new_bid(room_id, auction_id, auction.title, 
                            bidder.username, bid_amount, time_left);
            printf("[BID] User %s bid %.2f on auction %d\n", bidder.username, bid_amount, auction_id);
        }
        
        sprintf(response, "BID_SUCCESS|%d|%.2f|%d|%d\n", 
                auction_id, bid_amount, auction.total_bids, time_left);
    } else {
        sprintf(response, "BID_FAIL|%d|Bid failed\n", result);
    }
    
    send(client_socket, response, strlen(response), 0);
}void handle_buy_now(int client_socket, char *data) {
    int auction_id, user_id;
    sscanf(data, "%d|%d", &auction_id, &user_id);
    
    // ═══════════════════════════════════════════════════════
    // STEP 1: Get auction info FIRST
    // ═══════════════════════════════════════════════════════
    int room_id = 0;
    Auction auction;
    if (db_get_auction_with_room(auction_id, &auction, &room_id) != 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "BUY_NOW_FAIL|Auction not found\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 2: Check if seller trying to buy their own auction
    // ═══════════════════════════════════════════════════════
    if (auction.seller_id == user_id) {
        char response[BUFFER_SIZE];
        sprintf(response, "BUY_NOW_FAIL|Cannot buy your own auction\n");
        send(client_socket, response, strlen(response), 0);
        printf("[ERROR] User %d tried to buy their own auction %d\n", user_id, auction_id);
        return;
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 3: Execute buy now
    // ═══════════════════════════════════════════════════════
    int result = db_buy_now(auction_id, user_id);
    
    char response[BUFFER_SIZE];
    if (result == 0) {
        // BROADCAST WINNER
        db_get_auction_with_room(auction_id, &auction, &room_id);
        
        User winner;
        if (db_get_user(user_id, &winner) == 0) {
            printf("[BUY_NOW] Broadcasting winner to room %d: %s won auction %d\n", room_id, winner.username, auction_id);
            broadcast_auction_winner(room_id, auction_id, auction.title,
                                   winner.username, auction.buy_now_price, "Buy Now");
        }
        
        sprintf(response, "BUY_NOW_SUCCESS|%d\n", auction_id);
        send(client_socket, response, strlen(response), 0);
        printf("[BUY_NOW] Sent response to buyer (socket: %d)\n", client_socket);
        
        printf("[INFO] Buy now: User %d, Auction %d\n", user_id, auction_id);
        
        // ═══════════════════════════════════════════════════════
        // STEP 4: START NEXT AUCTION FROM QUEUE
        // ═══════════════════════════════════════════════════════
        
        printf("[QUEUE] Auction %d ended by buy now, checking queue in room %d...\n", 
               auction_id, room_id);
        
        // Clear current auction in queue state
        sqlite3_stmt *stmt;
        const char *sql_clear = "UPDATE room_queue_state SET current_auction_id = NULL "
                               "WHERE room_id = ?";
        if (sqlite3_prepare_v2(g_db, sql_clear, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, room_id);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Get delay
        int delay = queue_get_delay(room_id);
        if (delay <= 0) delay = 3;
        
        printf("[QUEUE] Waiting %d seconds before next auction...\n", delay);
        sleep(delay);
        
        // Start next auction
        int next_auction = queue_start_next_auction(room_id);
        
        if (next_auction > 0) {
            printf("[QUEUE] ✅ Started next auction: %d\n", next_auction);
            
            // Get details and broadcast
            Auction next;
            if (db_get_auction(next_auction, &next) == 0) {
                User seller;
                if (db_get_user(next.seller_id, &seller) == 0) {
                    char start_msg[BUFFER_SIZE];
                    sprintf(start_msg, "NOTIF_AUCTION_STARTED|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
                            next_auction, next.title, seller.username,
                            next.start_price, next.buy_now_price, 
                            next.min_increment, next.duration);
                    broadcast_to_room(room_id, start_msg, -1);
                    
                    printf("[BROADCAST] Next auction started notification sent\n");
                }
            }
        } else {
            printf("[QUEUE] 📭 No more auctions in queue for room %d\n", room_id);
            
            char empty_msg[] = "NOTIF_QUEUE_EMPTY\n";
            broadcast_to_room(room_id, empty_msg, -1);
        }
        // ═══════════════════════════════════════════════════════
        
    } else {
        const char *error_msg;
        switch(result) {
            case -1: error_msg = "Auction not available"; break;
            case -2: error_msg = "Buy now not available"; break;
            case -3: error_msg = "Insufficient balance"; break;
            default: error_msg = "Unknown error"; break;
        }
        sprintf(response, "BUY_NOW_FAIL|%s\n", error_msg);
        send(client_socket, response, strlen(response), 0);
    }
}
void handle_delete_auction(int client_socket, char *data) {
    int auction_id, user_id;
    sscanf(data, "%d|%d", &auction_id, &user_id);
    
    printf("[DEBUG] Delete auction request: auction_id=%d, user_id=%d\n", auction_id, user_id);
    
    // Get auction details
    Auction auction;
    if (db_get_auction(auction_id, &auction) != 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "DELETE_FAIL|Auction not found\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // Check if user is the seller
    if (auction.seller_id != user_id) {
        char response[BUFFER_SIZE];
        sprintf(response, "DELETE_FAIL|Only the creator can delete this auction\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // Check status - không cho xóa ended
    if (strcmp(auction.status, "ended") == 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "DELETE_FAIL|Cannot delete ended auctions\n");
        send(client_socket, response, strlen(response), 0);
        printf("[DEBUG] Delete failed: Auction ended\n");
        return;
    }
    
    // Check for bids (chỉ với active auctions)
    if (strcmp(auction.status, "active") == 0 && auction.total_bids > 0) {
        char response[BUFFER_SIZE];
        sprintf(response, "DELETE_FAIL|Cannot delete auction with bids\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    // Save room_id and status before deleting
    int room_id = auction.room_id;
    char old_status[20];
    strcpy(old_status, auction.status);
    
    // Delete auction
    int result = db_delete_auction(auction_id, user_id);
    
    char response[BUFFER_SIZE];
    if (result == 0) {
        // Broadcast deletion to room
        broadcast_auction_deleted(room_id, auction_id, auction.title, client_socket);
        
        sprintf(response, "DELETE_SUCCESS|Auction deleted successfully\n");
        printf("[INFO] User %d deleted auction %d (status: %s)\n", user_id, auction_id, old_status);
        
        // ═══════════════════════════════════════════════════════
        // NEW: If deleted active auction, start next in queue
        // ═══════════════════════════════════════════════════════
        if (strcmp(old_status, "active") == 0) {
            printf("[QUEUE] Active auction deleted, checking queue...\n");
            
            // Clear current auction in queue state
            sqlite3_stmt *stmt;
            const char *sql = "UPDATE room_queue_state SET current_auction_id = NULL "
                             "WHERE room_id = ?";
            if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, room_id);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            // Start next auction
            int next_auction = queue_start_next_auction(room_id);
            if (next_auction > 0) {
                printf("[QUEUE] ✅ Auto-started next auction: %d\n", next_auction);
                
                // Get details and broadcast
                Auction next;
                if (db_get_auction(next_auction, &next) == 0) {
                    User seller;
                    if (db_get_user(next.seller_id, &seller) == 0) {
                        char start_msg[BUFFER_SIZE];
                        sprintf(start_msg, "NOTIF_AUCTION_START|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
                                next_auction, next.title, seller.username,
                                next.start_price, next.buy_now_price, 
                                next.min_increment, next.duration);
                        broadcast_to_room(room_id, start_msg, -1);
                    }
                }
            } else {
                printf("[QUEUE] 📭 No more auctions in queue\n");
                char empty_msg[] = "NOTIF_QUEUE_EMPTY\n";
                broadcast_to_room(room_id, empty_msg, -1);
            }
        }
        // ═══════════════════════════════════════════════════════
        
    } else {
        sprintf(response, "DELETE_FAIL|Failed to delete auction\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}
void handle_bid_history(int client_socket, char *data) {
    int auction_id;
    sscanf(data, "%d", &auction_id);
    
    Bid bids[100];
    int count = db_get_bid_history(auction_id, bids, 100);
    
    char response[BUFFER_SIZE * 4];
    strcpy(response, "BID_HISTORY|");
    
    for (int i = 0; i < count; i++) {
        User user;
        char username[50] = "Unknown";
        if (db_get_user(bids[i].user_id, &user) == 0) {
            strncpy(username, user.username, sizeof(username)-1);
        }
        
        char bid_str[200];
        sprintf(bid_str, "%d;%s;%.2f;%ld|", 
                bids[i].bid_id, username, bids[i].bid_amount, bids[i].bid_time);
        strcat(response, bid_str);
    }
    
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
}
void handle_room_history(int client_socket, char *data) {
    int room_id;
    sscanf(data, "%d", &room_id);
    
    sqlite3_stmt *stmt;
    const char *sql = 
    "SELECT a.auction_id, a.title, a.start_price, a.current_price, "
    "  COALESCE((SELECT u.username FROM users u WHERE u.user_id = a.winner_id), 'No winner') as winner, "
    "  a.total_bids, "
    "  (SELECT COUNT(DISTINCT user_id) FROM bids b WHERE b.auction_id = a.auction_id) as participant_count, "
    "  a.status, "
    "  COALESCE((SELECT u.username FROM users u WHERE u.user_id = a.seller_id), 'Unknown') as seller, "
    "  COALESCE(a.win_method, '-') as win_method "  // ← THÊM DÒNG NÀY
    "FROM auctions a "
    "WHERE a.room_id = ? AND a.status = 'ended' "
    "ORDER BY a.end_time DESC";
    
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("[ERROR] SQL prepare failed: %s\n", sqlite3_errmsg(g_db));
        char response[BUFFER_SIZE];
        sprintf(response, "ERROR|Failed to get room history\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    sqlite3_bind_int(stmt, 1, room_id);
    
    char response[BUFFER_SIZE * 4] = "ROOM_HISTORY|";
    char temp[256];
    int count = 0;
   while (sqlite3_step(stmt) == SQLITE_ROW) {
    int auction_id = sqlite3_column_int(stmt, 0);
    const char *title = (const char*)sqlite3_column_text(stmt, 1);
    double start_price = sqlite3_column_double(stmt, 2);
    double final_price = sqlite3_column_double(stmt, 3);
    const char *winner = (const char*)sqlite3_column_text(stmt, 4);
    int total_bids = sqlite3_column_int(stmt, 5);
    int participants = sqlite3_column_int(stmt, 6);
    const char *status = (const char*)sqlite3_column_text(stmt, 7);
    const char *seller = (const char*)sqlite3_column_text(stmt, 8);
    const char *win_method = (const char*)sqlite3_column_text(stmt, 9);  // ← THÊM DÒNG NÀY
    
    // Format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status;seller;winMethod
    sprintf(temp, "%d;%s;%.2f;%.2f;%s;%d;%d;%s;%s;%s|",  // ← Thêm %s
            auction_id, title, start_price, final_price, winner, 
            total_bids, participants, status, seller, win_method);  // ← Thêm win_method
    strcat(response, temp);
    count++;
}
    
    sqlite3_finalize(stmt);
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
    
    printf("[INFO] Sent room history for room %d: %d auctions\n", room_id, count);
}void handle_auction_history(int client_socket, char *data) {
    int user_id;
    sscanf(data, "%d", &user_id);
    
    sqlite3_stmt *stmt;
    
    // Query auctions user participated in
   // Query auctions user participated in (both bid and buy_now)
const char *sql = 
    "SELECT DISTINCT "
    "  a.auction_id, "
    "  a.title, "
    "  a.start_price, "
    "  a.current_price, "
    "  COALESCE("
    "    (SELECT u.username FROM users u WHERE u.user_id = a.winner_id), "
    "    (SELECT u.username FROM users u WHERE u.user_id = a.current_bidder_id), "
    "    'No winner'"
    "  ) as winner, "
    "  (SELECT COUNT(*) FROM bids WHERE auction_id = a.auction_id AND user_id = ?) as user_bid_count, "
    "  a.total_bids, "
    "  (SELECT COUNT(DISTINCT user_id) FROM bids WHERE auction_id = a.auction_id) as participant_count, "
    "  a.status, "
    "  COALESCE(a.win_method, '-') as win_method "
    "FROM auctions a "
    "WHERE (a.auction_id IN (SELECT auction_id FROM bids WHERE user_id = ?) "
    "       OR a.winner_id = ?) "  // ← SPACE Ở CUỐI!
    "ORDER BY a.end_time DESC";
    
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("[ERROR] SQL prepare failed: %s\n", sqlite3_errmsg(g_db));
        char response[BUFFER_SIZE];
        sprintf(response, "ERROR|Failed to get auction history\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
sqlite3_bind_int(stmt, 2, user_id);
sqlite3_bind_int(stmt, 3, user_id);  // ← THÊM!
    
    char response[BUFFER_SIZE * 4] = "AUCTION_HISTORY|";
    char temp[256];
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
    int auction_id = sqlite3_column_int(stmt, 0);
    const char *title = (const char*)sqlite3_column_text(stmt, 1);
    double start_price = sqlite3_column_double(stmt, 2);
    double final_price = sqlite3_column_double(stmt, 3);
    const char *winner = (const char*)sqlite3_column_text(stmt, 4);
    int user_bid_count = sqlite3_column_int(stmt, 5);
    int total_bids = sqlite3_column_int(stmt, 6);
    int participants = sqlite3_column_int(stmt, 7);
    const char *status = (const char*)sqlite3_column_text(stmt, 8);
    const char *win_method = (const char*)sqlite3_column_text(stmt, 9);  // ← THÊM DÒNG NÀY
    
    // Format: auctionId;title;startPrice;finalPrice;winner;userBidCount;totalBids;participants;status;winMethod
    sprintf(temp, "%d;%s;%.2f;%.2f;%s;%d;%d;%d;%s;%s|",  // ← Thêm %s
            auction_id, title, start_price, final_price, winner,
            user_bid_count, total_bids, participants, status, win_method);  // ← Thêm win_method
    strcat(response, temp);
    count++;
}
    
    sqlite3_finalize(stmt);
    strcat(response, "\n");
    send(client_socket, response, strlen(response), 0);
    
    printf("[INFO] Sent auction history to user %d: %d auctions\n", user_id, count);
}
void handle_request(int client_socket, char *buffer) {
    char command[50], data[BUFFER_SIZE];
    
    char *delimiter = strchr(buffer, '|');
    if (delimiter) {
        int cmd_len = delimiter - buffer;
        strncpy(command, buffer, cmd_len);
        command[cmd_len] = '\0';
        strcpy(data, delimiter + 1);
    } else {
        strcpy(command, buffer);
        data[0] = '\0';
    }
    
    printf("[DEBUG] Command: %s\n", command);
    
    if (strcmp(command, "REGISTER") == 0) {
        handle_register(client_socket, data);
    } else if (strcmp(command, "LOGIN") == 0) {
        handle_login(client_socket, data);
    } else if (strcmp(command, "CREATE_ROOM") == 0) {
        handle_create_room(client_socket, data);
    } else if (strcmp(command, "LIST_ROOMS") == 0) {
        handle_list_rooms(client_socket);
    } else if (strcmp(command, "JOIN_ROOM") == 0) {
        handle_join_room(client_socket, data);
    } else if (strcmp(command, "LEAVE_ROOM") == 0) {
        handle_leave_room(client_socket, data);
    } else if (strcmp(command, "CREATE_AUCTION") == 0) {
        handle_create_auction(client_socket, data);
    } else if (strcmp(command, "LIST_AUCTIONS") == 0) {
        handle_list_auctions(client_socket, data);
    } else if (strcmp(command, "VIEW_AUCTION") == 0) {
        handle_view_auction(client_socket, data);
    } else if (strcmp(command, "SEARCH_AUCTIONS") == 0) {
        handle_search_auctions(client_socket, data);
    } else if (strcmp(command, "PLACE_BID") == 0) {
        handle_place_bid(client_socket, data);
    } else if (strcmp(command, "BUY_NOW") == 0) {
        handle_buy_now(client_socket, data);
    } else if (strcmp(command, "BID_HISTORY") == 0) {
        handle_bid_history(client_socket, data);
    } 
    else if (strcmp(command, "AUCTION_HISTORY") == 0) {
    handle_auction_history(client_socket, data);
}
else if (strcmp(command, "ROOM_HISTORY") == 0) {
    handle_room_history(client_socket, data);
}
        else if (strcmp(command, "SELLER_HISTORY") == 0) {
    handle_seller_history(client_socket, data);
}
    else if (strcmp(command, "DELETE_AUCTION") == 0) {
    handle_delete_auction(client_socket, data);
}
     else {
        char response[256];
        sprintf(response, "ERROR|Unknown command: %s\n", command);
        send(client_socket, response, strlen(response), 0);
    }
}