# 🚀 HƯỚNG DẪN NHANH - QT CLIENT

## 📦 TẢI VỀ

File `auction-qt-client.zip` chứa:
- ✅ Toàn bộ source code Qt
- ✅ Models (User, Room, Auction)
- ✅ NetworkManager (kết nối server)
- ✅ UI files (.ui)
- ✅ Hướng dẫn đầy đủ

## 🛠️ CÀI ĐẶT Qt

### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev build-essential
```

### Arch Linux:
```bash
sudo pacman -S qt6-base qt6-tools
```

### macOS:
```bash
brew install qt@6
```

### Windows:
Tải Qt Creator: https://www.qt.io/download-open-source

---

## 🏗️ BUILD PROJECT

### Cách 1: Qt Creator (Dễ nhất)

1. Giải nén `auction-qt-client.zip`
2. Mở Qt Creator
3. File → Open File or Project
4. Chọn `AuctionClientQt.pro`
5. Click "Configure Project"
6. Click "Build" (Ctrl+B)
7. Click "Run" (Ctrl+R)

### Cách 2: Command Line

```bash
# Giải nén
unzip auction-qt-client.zip
cd auction-qt-client

# Build
qmake AuctionClientQt.pro
make

# Run
./AuctionClientQt
```

---

## 🔌 KẾT NỐI VỚI SERVER

### Bước 1: Chạy Server

```bash
# Trong project cũ của bạn
cd /path/to/auction-modular
make run
```

Server chạy trên `127.0.0.1:8080`

### Bước 2: Chạy Qt Client

```bash
./AuctionClientQt
```

### Bước 3: Kết nối

1. **Trong Login Window:**
   - Host: `127.0.0.1`
   - Port: `8080`
   - Click "Connect"

2. **Đăng nhập:**
   - Username: `alice` (hoặc `bob`, `charlie`)
   - Password: `password123`
   - Click "Login"

---

## ✅ CHỨC NĂNG ĐÃ CÓ

### Authentication:
✅ Login
✅ Register
✅ Logout

### Room Management:
✅ List rooms
✅ Create room
✅ Join room
✅ Leave room

### Auction:
✅ List auctions
✅ Create auction
✅ Activate auction (start)
✅ Delete auction
✅ View details
✅ Real-time countdown

### Bidding:
✅ Place bid
✅ Buy now
✅ Bid history

### Real-time:
✅ New bid notifications
✅ Auction warnings (30s)
✅ Winner announcements
✅ User join/leave
✅ Auto refresh

---

## 🎮 TEST NHANH

### Terminal 1: Server
```bash
cd auction-modular
make run
```

### Terminal 2: Qt Client (Alice)
```bash
./AuctionClientQt
# Login: alice/password123
# Tạo room "Test Room"
# Tạo auction "iPhone"
# Activate auction
```

### Terminal 3: Qt Client (Bob)
```bash
./AuctionClientQt
# Login: bob/password123
# Join "Test Room"
# Bid vào iPhone
```

Xem real-time updates! 🎉

---

## 📁 CẤU TRÚC CODE

```
auction-qt-client/
├── main.cpp                 # Entry point
├── core/
│   └── networkmanager.*     # TCP socket, gửi/nhận lệnh
├── models/
│   ├── user.h               # Data class User
│   ├── room.h               # Data class Room
│   └── auction.h            # Data class Auction
├── windows/
│   ├── loginwindow.*        # Login UI
│   └── mainwindow.*         # Main UI
├── dialogs/
│   ├── bidplacedialog.*     # Đặt giá
│   ├── createroomdialog.*   # Tạo room
│   └── createauctiondialog.* # Tạo auction
└── utils/
    ├── constants.h          # Hằng số
    └── formatters.h         # Format tiền, thời gian
```

---

## 🐛 LỖI THƯỜNG GẶP

### 1. "Cannot find Qt"

```bash
# Set Qt path
export PATH="/usr/lib/qt6/bin:$PATH"
export QT_SELECT=qt6

# Hoặc dùng qmake trực tiếp
/usr/lib/qt6/bin/qmake AuctionClientQt.pro
```

### 2. "Connection refused"

- Kiểm tra server đang chạy: `ps aux | grep auction_server`
- Kiểm tra port: `netstat -tlnp | grep 8080`
- Thử: `telnet 127.0.0.1 8080`

### 3. Build lỗi C++17

Sửa file `.pro`:
```qmake
CONFIG += c++14  # Thay vì c++17
```

---

## 📝 CÁC FILES QUAN TRỌNG

### NetworkManager.cpp
- Gửi/nhận commands qua socket
- Parse responses từ server
- Emit Qt signals cho UI

### MainWindow.cpp  
- UI chính: rooms + auctions
- Handle button clicks
- Update real-time

### LoginWindow.cpp
- Connect server
- Login/Register

---

## 🎯 ĐIỀU CHỈNH

### Đổi IP Server

File: `utils/constants.h`
```cpp
const QString DEFAULT_HOST = "192.168.1.100";  // Đổi IP
const int DEFAULT_PORT = 8080;
```

### Thêm Chức Năng Mới

1. Thêm command trong `NetworkManager`
2. Thêm signal
3. Parse response
4. Connect signal với UI slot

---

## 📖 ĐỌC THÊM

- `README.md` - Hướng dẫn đầy đủ (English)
- Server protocol - Xem trong server code
- Qt Documentation - https://doc.qt.io/

---

## ✅ CHECKLIST

Trước khi chạy:
- [ ] Qt đã cài
- [ ] Server đang chạy
- [ ] Build thành công
- [ ] Kết nối được server
- [ ] Login được

---

## 🎉 XONG!

Client Qt đã sẵn sàng với đầy đủ chức năng như CLI client!

Có vấn đề? Check:
1. Server logs
2. Client Activity Log
3. Debug output: `QT_LOGGING_RULES="*.debug=true" ./AuctionClientQt`

**Chúc vui vẻ!** 🚀
