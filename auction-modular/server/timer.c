#include "../shared/config.h"
#include "database.h"
#include "broadcast.h"
#include "queue.h"              // ← ADD THIS
#include "../shared/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"


static int timer_running = 1;

void* auction_timer_thread(void *arg) {
    (void)arg;
    
    // Track which auctions got 30s warning already
    int warned_auctions[1000] = {0};
    int warned_count = 0;
    
    while (timer_running) {
        sleep(1); // Check every second
        
        Auction auctions[100];
        int count = db_get_all_active_auctions(auctions, 100);
        
        time_t now = time(NULL);
        
        for (int i = 0; i < count; i++) {
            int time_left = auctions[i].end_time - now;
            
            // ═══════════════════════════════════════════════════════
            // AUCTION ENDED
            // ═══════════════════════════════════════════════════════
            if (time_left <= 0 && strcmp(auctions[i].status, "active") == 0) {
                printf("[TIMER] Auction %d time expired (time_left=%d)\n", 
                       auctions[i].auction_id, time_left);
                
                int room_id = 0;
                Auction temp;
                db_get_auction_with_room(auctions[i].auction_id, &temp, &room_id);
                
                if (auctions[i].current_bidder_id > 0) {
                    // Has winner
                    db_end_auction(auctions[i].auction_id, auctions[i].current_bidder_id, "bid");
                    
                    User winner;
                    if (db_get_user(auctions[i].current_bidder_id, &winner) == 0) {
                        // BROADCAST WINNER TO ALL IN ROOM
                        broadcast_auction_winner(room_id, auctions[i].auction_id, 
                                               auctions[i].title, winner.username, 
                                               auctions[i].current_price, "Time Ended");
                        
                        printf("[TIMER] ✅ Auction %d ended - Winner: %s (%.0f VND)\n", 
                               auctions[i].auction_id, winner.username, auctions[i].current_price);
                    }
                } else {
                    // No bids
                    db_end_auction(auctions[i].auction_id, 0, "-");
                    
                    // Broadcast no bids message
                    char no_bid_msg[BUFFER_SIZE];
                    sprintf(no_bid_msg, "NOTIF_WINNER|%d|%s|No Winner|0.00|-\n",
                            auctions[i].auction_id, auctions[i].title);
                    broadcast_to_room(room_id, no_bid_msg, -1);
                    printf("[TIMER] ❌ Auction %d ended - No bids\n", auctions[i].auction_id);
                }
                
                // ═══════════════════════════════════════════════════════
                // NEW: START NEXT AUCTION FROM QUEUE
                // ═══════════════════════════════════════════════════════
                
                printf("[QUEUE] Checking for next auction in room %d...\n", room_id);
                
                // Wait a bit before starting next (configurable delay)
                int delay = queue_get_delay(room_id);
                if (delay <= 0) delay = 3; // Default 3 seconds
                
                printf("[QUEUE] Waiting %d seconds before next auction...\n", delay);
                sleep(delay);
                
                // Start next auction in queue
                int result = queue_start_next_auction(room_id);
                
                if (result > 0) {
                    // Successfully started next auction
                    Auction next_auction;
                    if (db_get_auction(result, &next_auction) == 0) {
                        User seller;
                        if (db_get_user(next_auction.seller_id, &seller) == 0) {
                            printf("[QUEUE] ✅ Started next auction: %d (%s) by %s\n",
                                   result, next_auction.title, seller.username);
                            
                            // Broadcast: Next auction started
                            char start_msg[BUFFER_SIZE];
                            sprintf(start_msg, "NOTIF_AUCTION_START|%d|%s|%s|%.2f|%.2f|%.2f|%d\n",
                                    next_auction.auction_id, next_auction.title, seller.username,
                                    next_auction.start_price, next_auction.buy_now_price,
                                    next_auction.min_increment, next_auction.duration);
                            broadcast_to_room(room_id, start_msg, -1);
                        }
                    }
                } else if (result == 0) {
                    // No more auctions in queue
                    printf("[QUEUE] 📭 No more auctions in queue for room %d\n", room_id);
                    
                    // Broadcast: Queue empty
                    char empty_msg[] = "NOTIF_QUEUE_EMPTY\n";
                    broadcast_to_room(room_id, empty_msg, -1);
                } else {
                    // Error starting next auction
                    printf("[QUEUE] ❌ Failed to start next auction in room %d\n", room_id);
                }
                
                // ═══════════════════════════════════════════════════════
                
                // Reset warning flag for this auction
                for (int j = 0; j < warned_count; j++) {
                    if (warned_auctions[j] == auctions[i].auction_id) {
                        warned_auctions[j] = 0;
                    }
                }
            }
            // 30 SECOND WARNING
            else if (time_left <= 30 && time_left > 0) {
                // Check if already warned
                int already_warned = 0;
                for (int j = 0; j < warned_count; j++) {
                    if (warned_auctions[j] == auctions[i].auction_id) {
                        already_warned = 1;
                        break;
                    }
                }
                
                if (!already_warned) {
                    int room_id = 0;
                    Auction temp;
                    db_get_auction_with_room(auctions[i].auction_id, &temp, &room_id);
                    
                    // BROADCAST WARNING TO ALL IN ROOM
                    broadcast_30s_warning(room_id, auctions[i].auction_id, auctions[i].title, time_left);
                    
                    // Mark as warned
                    if (warned_count < 1000) {
                        warned_auctions[warned_count++] = auctions[i].auction_id;
                    }
                    
                    printf("[TIMER] ⚠️  30s warning for auction %d (%s) - %ds left\n", 
                           auctions[i].auction_id, auctions[i].title, time_left);
                }
            }
        }
    }
    
    printf("[TIMER] Auction timer thread stopped\n");
    return NULL;
}

void start_auction_timer() {
    pthread_t timer_thread;
    if (pthread_create(&timer_thread, NULL, auction_timer_thread, NULL) != 0) {
        fprintf(stderr, "[ERROR] Failed to create timer thread\n");
        return;
    }
    pthread_detach(timer_thread);
    printf("[INFO] Auction timer initialized with queue support\n");
}