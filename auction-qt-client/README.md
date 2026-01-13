# 🎪 Auction System - Qt Client

Qt GUI Client for Auction System with full functionality.

## 📋 Features

✅ **Authentication**
- Login/Register with server
- Session management
- Balance tracking

✅ **Room Management**
- List all rooms
- Create new rooms
- Join/Leave rooms
- View participants

✅ **Auction Features**
- List auctions in room
- Create new auctions
- View auction details
- Place bids
- Buy now (instant purchase)
- Delete auctions (seller only)
- Activate auctions
- Real-time countdown timers

✅ **Real-time Updates**
- Live bid notifications
- Auction warnings (30s before end)
- Winner announcements
- User join/leave notifications
- Automatic auction list refresh

✅ **User Interface**
- Clean, intuitive design
- Separate panels for rooms/auctions
- Activity log
- Status indicators
- Error handling with user-friendly messages

---

## 🛠️ Requirements

### System Requirements:
- **Qt 6.x** (Qt 5.15+ also works)
- **C++17** compiler (GCC 9+, Clang 10+, MSVC 2019+)
- **CMake 3.16+** or **qmake**

### Install Qt:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev build-essential
```

**Arch Linux:**
```bash
sudo pacman -S qt6-base qt6-tools
```

**macOS:**
```bash
brew install qt@6
```

**Windows:**
Download Qt from: https://www.qt.io/download-open-source

---

## 🚀 Build Instructions

### Method 1: Using qmake (Recommended)

```bash
# Navigate to project directory
cd auction-qt-client

# Generate Makefile
qmake AuctionClientQt.pro

# Build
make

# Run
./AuctionClientQt
```

### Method 2: Using Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select `AuctionClientQt.pro`
4. Configure project (select kit)
5. Click "Build" (Ctrl+B)
6. Click "Run" (Ctrl+R)

### Method 3: Using CMake

```bash
cd auction-qt-client
mkdir build && cd build
cmake ..
make
./AuctionClientQt
```

---

## 🔌 Connecting to Server

### Step 1: Start the Server

Make sure the auction server is running:

```bash
# In the server directory
cd /path/to/auction-modular
make run
```

Server will start on `127.0.0.1:8080` by default.

### Step 2: Launch Qt Client

```bash
./AuctionClientQt
```

### Step 3: Connect

In the login window:

1. **Server Settings:**
   - Host: `127.0.0.1` (for local server)
   - Port: `8080`
   - Click "Connect"

2. **Login:**
   - Username: `alice`, `bob`, or `charlie` (test accounts)
   - Password: `password123`
   - Click "Login"

**OR Register a new account:**
   - Enter new username (min 3 chars)
   - Enter password (min 6 chars)
   - Click "Register"
   - Then login with new credentials

---

## 📖 Usage Guide

### Creating a Room

1. Click "Create Room" button
2. Fill in:
   - Room name
   - Description
   - Max participants (default: 10)
   - Duration in seconds (default: 3600)
3. Click "Create"

### Joining a Room

1. Select a room from the list
2. Click "Join Room"
3. Auction list will update automatically

### Creating an Auction

1. Make sure you're in a room
2. Click "Create Auction"
3. Fill in:
   - Title
   - Description
   - Start price (VND)
   - Buy now price (optional)
   - Minimum bid increment (default: 100,000 VND)
   - Duration in seconds
4. Click "Create"
5. **IMPORTANT**: Click "Activate" to start the auction

### Placing a Bid

1. Select an active auction
2. Click "Place Bid"
3. Enter your bid amount
4. Click "Place Bid"

**Notes:**
- Bid must be higher than current price + minimum increment
- You cannot bid on your own auctions
- You need sufficient balance

### Buy Now

1. Select an auction with "Buy Now" price
2. Click "Buy Now"
3. Confirm purchase
4. Auction ends immediately

---

## 🎮 Testing with Multiple Clients

### Scenario: Full Auction Flow

**Terminal 1: Server**
```bash
cd auction-modular
make run
```

**Terminal 2: Qt Client 1 (Alice - Seller)**
```bash
./AuctionClientQt
# Login as: alice/password123
# Create room "Tech Auction"
# Create auction "iPhone 15 Pro"
# Activate auction
```

**Terminal 3: Qt Client 2 (Bob - Bidder)**
```bash
./AuctionClientQt
# Login as: bob/password123
# Join "Tech Auction"
# Bid on iPhone
```

**Terminal 4: Qt Client 3 (Charlie - Bidder)**
```bash
./AuctionClientQt
# Login as: charlie/password123
# Join "Tech Auction"
# Bid higher than Bob
```

Watch real-time updates in all clients!

---

## 🐛 Troubleshooting

### Cannot connect to server

**Problem:** "Connection refused"

**Solutions:**
1. Make sure server is running:
   ```bash
   ps aux | grep auction_server
   ```

2. Check server is listening:
   ```bash
   netstat -tlnp | grep 8080
   ```

3. Try connecting with telnet:
   ```bash
   telnet 127.0.0.1 8080
   ```

4. Check firewall:
   ```bash
   sudo ufw status
   sudo ufw allow 8080/tcp
   ```

### Build errors

**Problem:** "Cannot find Qt"

**Solution:**
```bash
# Set Qt path
export PATH="/usr/lib/qt6/bin:$PATH"
export QT_SELECT=qt6

# Or specify qmake directly
/usr/lib/qt6/bin/qmake AuctionClientQt.pro
```

**Problem:** "C++17 not supported"

**Solution:**
Update compiler or edit .pro file:
```qmake
CONFIG += c++14  # Change to c++14 if needed
```

### Runtime errors

**Problem:** UI doesn't update

**Solution:**
- Click "Refresh" buttons
- Check Activity Log for errors
- Reconnect to server

**Problem:** "Bid failed"

**Possible reasons:**
- Insufficient balance
- Bid too low (must be current + increment)
- Auction not active
- You're the seller

---

## 📂 Project Structure

```
auction-qt-client/
├── main.cpp                    # Entry point
├── AuctionClientQt.pro         # Qt project file
│
├── core/                       # Core logic
│   ├── networkmanager.h/cpp    # Socket communication
│
├── models/                     # Data models
│   ├── user.h                  # User model
│   ├── room.h                  # Room model
│   └── auction.h               # Auction model
│
├── windows/                    # Main windows
│   ├── loginwindow.h/cpp/ui    # Login screen
│   └── mainwindow.h/cpp/ui     # Main application
│
├── dialogs/                    # Modal dialogs
│   ├── bidplacedialog.*        # Bid placement
│   ├── createroomdialog.*      # Room creation
│   └── createauctiondialog.*   # Auction creation
│
├── utils/                      # Utilities
│   ├── constants.h             # Constants
│   └── formatters.h            # Formatting helpers
│
└── README.md                   # This file
```

---

## 🔐 Default Test Accounts

The server comes with pre-configured test accounts:

| Username | Password | Balance |
|----------|----------|---------|
| alice | password123 | 50,000,000 VND |
| bob | password123 | 50,000,000 VND |
| charlie | password123 | 50,000,000 VND |

---

## 🌐 Network Protocol

The client communicates with the server using a simple text-based protocol over TCP.

### Command Format:
```
COMMAND|param1|param2|...\n
```

### Example Commands:
```
LOGIN|alice|password123
CREATE_ROOM|1|My Room|Description|10|3600
PLACE_BID|1|2|12000000
```

### Response Format:
```
RESPONSE_TYPE|data1|data2|...\n
```

### Example Responses:
```
LOGIN_SUCCESS|1|alice|50000000.0
BID_SUCCESS
ROOM_LIST|1,Room1,Desc,alice,5,10,3;2,Room2,Desc,bob,2,10,1
```

### Notifications (Real-time):
```
NOTIF_BID|1|12000000|bob
NOTIF_WARNING|1|30
NOTIF_WINNER|1|alice|15000000
```

---

## 🚧 Known Limitations

- No persistent settings (server host/port)
- No sound notifications (can be added)
- No chat between users
- No auction history view in UI (command exists)
- No search functionality in UI yet

---

## 🔮 Future Enhancements

Possible improvements:
- [ ] Dark mode toggle
- [ ] Sound effects for notifications
- [ ] System tray icon
- [ ] Save/restore window state
- [ ] User avatars
- [ ] Auction images
- [ ] Chat functionality
- [ ] Advanced search/filters
- [ ] Statistics dashboard
- [ ] Export auction history

---

## 📝 Development Notes

### Adding New Features

1. **Add command to NetworkManager:**
   ```cpp
   void NetworkManager::sendNewCommand(params) {
       QString command = QString("NEW_COMMAND|%1|%2")
           .arg(param1).arg(param2);
       sendCommand(command);
   }
   ```

2. **Add signal:**
   ```cpp
   signals:
       void newCommandResponse(data);
   ```

3. **Parse response in `parseResponse()`:**
   ```cpp
   else if (command == "NEW_RESPONSE") {
       emit newCommandResponse(parts[1]);
   }
   ```

4. **Connect in UI:**
   ```cpp
   connect(network, &NetworkManager::newCommandResponse,
           this, &MainWindow::onNewCommandResponse);
   ```

### Code Style

- Use Qt naming conventions
- Member variables: `camelCase`
- Class names: `PascalCase`
- Signals/Slots: prefix with `on`
- Constants: `UPPER_CASE`

---

## 📄 License

This project is part of the Auction System educational project.

---

## 🆘 Support

For issues or questions:
1. Check server logs
2. Check client Activity Log
3. Enable debug output:
   ```bash
   QT_LOGGING_RULES="*.debug=true" ./AuctionClientQt
   ```

---

## ✅ Testing Checklist

Before deployment, test:
- [ ] Connect to server
- [ ] Login/Register
- [ ] Create room
- [ ] Join/Leave room
- [ ] Create auction
- [ ] Activate auction
- [ ] Place bid
- [ ] Buy now
- [ ] Real-time notifications
- [ ] Countdown timers
- [ ] Multiple clients simultaneously
- [ ] Disconnect/Reconnect
- [ ] Error handling

---

**Happy Auctioning!** 🎉
