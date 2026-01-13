-- Users table
    CREATE TABLE IF NOT EXISTS users (
        user_id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT UNIQUE NOT NULL,
        password TEXT NOT NULL,
        balance REAL DEFAULT 50000000.0,
        is_active INTEGER DEFAULT 1,
        created_at INTEGER DEFAULT (strftime('%s', 'now'))
    );

    -- Rooms table
    CREATE TABLE IF NOT EXISTS rooms (
        room_id INTEGER PRIMARY KEY AUTOINCREMENT,
        room_name TEXT NOT NULL,
        description TEXT,
        created_by INTEGER NOT NULL,
        max_participants INTEGER DEFAULT 10,
        current_participants INTEGER DEFAULT 0,
        total_auctions INTEGER DEFAULT 0,
        created_at INTEGER DEFAULT (strftime('%s', 'now')),
        end_time INTEGER,
        status TEXT DEFAULT 'active',
        FOREIGN KEY (created_by) REFERENCES users(user_id)
    );

    -- Auctions table (with queue support)
    CREATE TABLE IF NOT EXISTS auctions (
        auction_id INTEGER PRIMARY KEY AUTOINCREMENT,
        seller_id INTEGER NOT NULL,
        room_id INTEGER NOT NULL,
        title TEXT NOT NULL,
        description TEXT,
        start_price REAL NOT NULL,
        current_price REAL NOT NULL,
        buy_now_price REAL,
        min_increment REAL DEFAULT 10000.0,
        current_bidder_id INTEGER,
        start_time INTEGER,
        end_time INTEGER,
        duration INTEGER,
        total_bids INTEGER DEFAULT 0,
        status TEXT DEFAULT 'waiting',
        winner_id INTEGER,
        win_method TEXT,
        queue_position INTEGER DEFAULT 0,
        is_queued INTEGER DEFAULT 0,
        FOREIGN KEY (seller_id) REFERENCES users(user_id),
        FOREIGN KEY (room_id) REFERENCES rooms(room_id)
    );

    -- Bids table
    CREATE TABLE IF NOT EXISTS bids (
        bid_id INTEGER PRIMARY KEY AUTOINCREMENT,
        auction_id INTEGER NOT NULL,
        user_id INTEGER NOT NULL,
        bid_amount REAL NOT NULL,
        bid_time INTEGER DEFAULT (strftime('%s', 'now')),
        FOREIGN KEY (auction_id) REFERENCES auctions(auction_id),
        FOREIGN KEY (user_id) REFERENCES users(user_id)
    );

    -- User-Room relationship
    CREATE TABLE IF NOT EXISTS user_rooms (
        user_id INTEGER NOT NULL,
        room_id INTEGER NOT NULL,
        joined_at INTEGER DEFAULT (strftime('%s', 'now')),
        PRIMARY KEY (user_id, room_id),
        FOREIGN KEY (user_id) REFERENCES users(user_id),
        FOREIGN KEY (room_id) REFERENCES rooms(room_id)
    );

    -- Pending notifications for offline users
    CREATE TABLE IF NOT EXISTS pending_notifications (
        notif_id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER NOT NULL,
        notification_type TEXT NOT NULL,
        auction_id INTEGER,
        room_id INTEGER,
        message TEXT NOT NULL,
        created_at INTEGER DEFAULT (strftime('%s', 'now')),
        is_read INTEGER DEFAULT 0,
        FOREIGN KEY (user_id) REFERENCES users(user_id),
        FOREIGN KEY (auction_id) REFERENCES auctions(auction_id),
        FOREIGN KEY (room_id) REFERENCES rooms(room_id)
    );

    -- User session tracking
    CREATE TABLE IF NOT EXISTS user_sessions (
        session_id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER NOT NULL,
        session_token TEXT UNIQUE,
        last_seen INTEGER DEFAULT (strftime('%s', 'now')),
        is_online INTEGER DEFAULT 1,
        FOREIGN KEY (user_id) REFERENCES users(user_id)
    );

    -- Auction queue metadata per room
    CREATE TABLE IF NOT EXISTS room_queue_state (
        room_id INTEGER PRIMARY KEY,
        current_auction_id INTEGER,
        next_auction_id INTEGER,
        queue_mode TEXT DEFAULT 'manual',
        auto_start_delay INTEGER DEFAULT 30,
        FOREIGN KEY (room_id) REFERENCES rooms(room_id),
        FOREIGN KEY (current_auction_id) REFERENCES auctions(auction_id),
        FOREIGN KEY (next_auction_id) REFERENCES auctions(auction_id)
    );

    -- Indexes for performance
    CREATE INDEX IF NOT EXISTS idx_auctions_room ON auctions(room_id);
    CREATE INDEX IF NOT EXISTS idx_auctions_seller ON auctions(seller_id);
    CREATE INDEX IF NOT EXISTS idx_auctions_status ON auctions(status);
    CREATE INDEX IF NOT EXISTS idx_auctions_queue ON auctions(room_id, queue_position, is_queued);
    CREATE INDEX IF NOT EXISTS idx_bids_auction ON bids(auction_id);
    CREATE INDEX IF NOT EXISTS idx_bids_user ON bids(user_id);
    CREATE INDEX IF NOT EXISTS idx_notifications_user ON pending_notifications(user_id, is_read);
    CREATE INDEX IF NOT EXISTS idx_sessions_user ON user_sessions(user_id, is_online);

    -- Insert test data
    INSERT OR IGNORE INTO users (user_id, username, password, balance) VALUES 
    (1, 'alice', 'password123', 50000000.0),
    (2, 'bob', 'password123', 50000000.0),
    (3, 'charlie', 'password123', 50000000.0);