echo "Cleaning test rooms from database..."



#!/bin/bash
echo "Cleaning test rooms from database..."

# Sử dụng sqlite3 command
sqlite3 auction.db << SQL
-- Xóa phòng không có auction
DELETE FROM room_queue_state WHERE room_id IN (
    SELECT r.room_id FROM rooms r 
    LEFT JOIN auctions a ON r.room_id = a.room_id 
    WHERE a.auction_id IS NULL
);

DELETE FROM rooms WHERE room_id NOT IN (
    SELECT DISTINCT room_id FROM auctions
);

-- Hiển thị kết quả
SELECT COUNT(*) as remaining_rooms FROM rooms;
SQL

echo "Done! Remaining rooms:"
sqlite3 auction.db "SELECT room_id, room_name FROM rooms;"
