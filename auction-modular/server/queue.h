#ifndef QUEUE_H
#define QUEUE_H

#include "../shared/types.h"

// Queue management functions
int queue_init_room(int room_id, const char *mode);
int queue_add_auction(int room_id, int auction_id);
int queue_remove_auction(int room_id, int auction_id);
int queue_get_next_auction(int room_id);
int queue_get_current_auction(int room_id);
int queue_start_next_auction(int room_id);
int queue_reorder_auctions(int room_id, int *auction_ids, int count);
int queue_get_position(int room_id, int auction_id);
int queue_get_all_in_room(int room_id, Auction *auctions, int max_count);

// Auto-queue management
void* queue_auto_processor_thread(void *arg);
void start_queue_processor();
void stop_queue_processor();

// Queue state queries
int queue_is_auto_mode(int room_id);
int queue_get_delay(int room_id);
int queue_set_mode(int room_id, const char *mode);
int queue_set_delay(int room_id, int delay_seconds);


#endif