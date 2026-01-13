# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -I.
SERVER_LDFLAGS = -lsqlite3 -lpthread
CLIENT_LDFLAGS = -pthread

# Directories
SERVER_DIR = server
CLIENT_DIR = client
SHARED_DIR = shared

# Server files
SERVER_SRCS = $(SERVER_DIR)/main.c \
              $(SERVER_DIR)/database.c \
              $(SERVER_DIR)/handlers.c \
              $(SERVER_DIR)/network.c \
              $(SERVER_DIR)/broadcast.c \
              $(SERVER_DIR)/timer.c \
              $(SERVER_DIR)/queue.c \
              $(SERVER_DIR)/notifications.c

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
SERVER_BIN = auction_server

# Client files
CLIENT_SRCS = $(CLIENT_DIR)/main.c \
              $(CLIENT_DIR)/network.c \
              $(CLIENT_DIR)/ui.c \
              $(CLIENT_DIR)/features.c \
              $(CLIENT_DIR)/notifications.c

CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
CLIENT_BIN = auction_client

# Build all
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server
$(SERVER_BIN): $(SERVER_OBJS)
	@echo "🔨 Linking server..."
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(SERVER_BIN) $(SERVER_LDFLAGS)
	@echo "✅ Server built successfully with queue system!"

# Build client
$(CLIENT_BIN): $(CLIENT_OBJS)
	@echo "🔨 Linking client..."
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $(CLIENT_BIN) $(CLIENT_LDFLAGS)
	@echo "✅ Client built successfully!"

# Compile .c to .o
%.o: %.c
	@echo "📦 Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Initialize database with queue system
init-db:
	@echo "📊 Initializing database with queue system..."
	@sqlite3 auction.db < schema.sql
	@echo "✅ Database initialized!"
	@echo ""
	@echo "Features enabled:"
	@echo "  ✅ User management"
	@echo "  ✅ Room & Auction system"
	@echo "  ✅ Auction Queue (auto/manual modes)"
	@echo "  ✅ Pending Notifications"
	@echo "  ✅ Disconnect Handling"
	@echo ""
	@echo "Test accounts:"
	@echo "  Username: alice   Password: password123"
	@echo "  Username: bob     Password: password123"
	@echo "  Username: charlie Password: password123"

# Setup (clean + init + build)
setup: clean init-db all
	@echo ""
	@echo "🎉 Setup complete!"
	@echo ""
	@echo "🚀 Quick Start:"
	@echo "  Terminal 1: make run          # Start server"
	@echo "  Terminal 2: make run-client   # Start client"
	@echo ""
	@echo "📖 Read QUEUE_GUIDE.md for queue system usage"

# Clean everything
clean:
	@echo "🧹 Cleaning..."
	@rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BIN) $(CLIENT_BIN)
	@echo "✅ Cleaned!"

# Clean build files only (keep database)
clean-build:
	@echo "🧹 Cleaning build files..."
	@rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BIN) $(CLIENT_BIN)
	@echo "✅ Build files cleaned!"

# Clean database only
clean-db:
	@echo "🧹 Cleaning database..."
	@rm -f auction.db auction.db.backup.*
	@echo "✅ Database cleaned!"

# Run server
run: $(SERVER_BIN)
	@echo "🚀 Starting server with queue system..."
	@echo ""
	./$(SERVER_BIN)

# Run client
run-client: $(CLIENT_BIN)
	@echo "🚀 Starting client..."
	./$(CLIENT_BIN)

# Demo queue system
demo-queue:
	@echo "🎪 Running queue system demo..."
	@if [ ! -f demo_queue.sh ]; then \
		echo "❌ demo_queue.sh not found!"; \
		exit 1; \
	fi
	@chmod +x demo_queue.sh
	@./demo_queue.sh

# Test connection
test:
	@echo "🧪 Testing server connection..."
	@if nc -z 127.0.0.1 8080 2>/dev/null; then \
		echo "✅ Server is running on port 8080"; \
	else \
		echo "❌ Server is not running"; \
		echo "   Start server with: make run"; \
		exit 1; \
	fi

# Check database status
check-db:
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "  📊 DATABASE STATUS"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@if [ -f auction.db ]; then \
		echo ""; \
		echo "✅ Database exists"; \
		echo ""; \
		echo "📋 Tables:"; \
		sqlite3 auction.db ".tables"; \
		echo ""; \
		echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "👥 Users:"; \
		sqlite3 auction.db "SELECT user_id, username, balance FROM users;" -header -column; \
		echo ""; \
		echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "🏠 Rooms:"; \
		sqlite3 auction.db "SELECT COUNT(*) as total_rooms FROM rooms;" -header -column; \
		echo ""; \
		echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "🎪 Auctions:"; \
		sqlite3 auction.db "SELECT COUNT(*) as total, status FROM auctions GROUP BY status;" -header -column 2>/dev/null || echo "No auctions"; \
		echo ""; \
		echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "📦 Queue:"; \
		sqlite3 auction.db "SELECT COUNT(*) as queued FROM auctions WHERE is_queued = 1;" -header -column 2>/dev/null || echo "Queue system ready"; \
		echo ""; \
		echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "🔔 Notifications:"; \
		sqlite3 auction.db "SELECT COUNT(*) as pending FROM pending_notifications WHERE is_read = 0;" -header -column 2>/dev/null || echo "No pending notifications"; \
		echo ""; \
	else \
		echo ""; \
		echo "❌ Database not found"; \
		echo "   Initialize with: make init-db"; \
		echo ""; \
	fi

# Help
help:
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "  AUCTION SYSTEM v2.1 - BUILD COMMANDS"
	@echo "  With Queue System & Disconnect Handling  🎪💔"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo ""
	@echo "📦 BUILD:"
	@echo "  make              - Build server and client"
	@echo "  make setup        - Clean + Init DB + Build all"
	@echo ""
	@echo "🗄️  DATABASE:"
	@echo "  make init-db      - Initialize database (with queue)"
	@echo "  make check-db     - Check database status"
	@echo "  make clean-db     - Remove database file"
	@echo ""
	@echo "🚀 RUN:"
	@echo "  make run          - Start server (port 8080)"
	@echo "  make run-client   - Start client"
	@echo "  make demo-queue   - Run queue system demo"
	@echo ""
	@echo "🧪 TEST:"
	@echo "  make test         - Test server connection"
	@echo ""
	@echo "🧹 CLEAN:"
	@echo "  make clean        - Remove all build files & binaries"
	@echo "  make clean-build  - Remove build files (keep DB)"
	@echo "  make clean-db     - Remove database only"
	@echo ""
	@echo "📚 HELP:"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo ""
	@echo "🎯 QUICK START:"
	@echo "  1. make setup       # Setup everything"
	@echo "  2. make run         # Terminal 1: Start server"
	@echo "  3. make run-client  # Terminal 2: Start client"
	@echo ""
	@echo "📖 DOCUMENTATION:"
	@echo "  • QUEUE_GUIDE.md    - Queue system usage"
	@echo "  • FRIEND_GUIDE.md   - Client setup guide"
	@echo ""

.PHONY: all clean clean-build clean-db init-db run run-client setup help test check-db demo-queue