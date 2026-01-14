#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>
#include <QStatusBar>
#include <QTime>
#include <QSplitter>
#include <QScrollBar>
#include <QInputDialog>
#include <QTextCursor>
#include "../dialogs/bidplacedialog.h"
#include "../dialogs/createroomdialog.h"
#include "../dialogs/createauctiondialog.h"
#include "../utils/formatters.h"
#include <algorithm>
MainWindow::MainWindow(NetworkManager *net, const User &user, QWidget *parent)
    : QMainWindow(parent),
      network(net),
      currentUser(user),
      countdownTimer(new QTimer(this)),
      warningCheckTimer(new QTimer(this)) // ← DẤU PHẨY, không phải dấu chấm phẩy
{                                         // ← DẤU { PHẢI Ở ĐÂY
    // Setup countdown timer
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdowns);
    countdownTimer->start(1000);

    // Setup warning timer
    connect(warningCheckTimer, &QTimer::timeout, this, &MainWindow::checkAuctionWarnings);
    warningCheckTimer->start(1000);

    setupUI();

    // Connect ALL signals
    connect(network, &NetworkManager::auctionStarted, this, &MainWindow::onAuctionStarted);
    connect(network, &NetworkManager::searchResultsReceived, this, &MainWindow::onSearchResultsReceived);
    connect(network, &NetworkManager::sellerHistoryReceived, // ← THÊM
            this, &MainWindow::onSellerHistoryReceived);
    connect(network, &NetworkManager::roomHistoryReceived, // ← THÊM
            this, &MainWindow::onRoomHistoryReceived);
    connect(network, &NetworkManager::auctionDeletedBroadcast,
            this, &MainWindow::onAuctionDeletedBroadcast);
    connect(network, &NetworkManager::roomListReceived, this, &MainWindow::onRoomListReceived);
    connect(network, &NetworkManager::auctionListReceived, this, &MainWindow::onAuctionListReceived);
    connect(network, &NetworkManager::joinedRoom, this, &MainWindow::onJoinedRoom);
    connect(network, &NetworkManager::leftRoom, this, &MainWindow::onLeftRoom);
    connect(network, &NetworkManager::roomCreated, this, &MainWindow::onRoomCreated);
    connect(network, &NetworkManager::auctionCreated, this, &MainWindow::onAuctionCreated);
    connect(network, &NetworkManager::auctionActivated, this, &MainWindow::onAuctionActivated);
    connect(network, &NetworkManager::bidPlaced, this, &MainWindow::onBidPlaced);
    connect(network, &NetworkManager::buyNowSuccess, this, &MainWindow::onBuyNowSuccess);
    connect(network, &NetworkManager::balanceUpdated, this, &MainWindow::onBalanceUpdated);
    connect(network, &NetworkManager::auctionDeleted, this, &MainWindow::onAuctionDeleted);
    connect(network, &NetworkManager::auctionDetails, this, &MainWindow::onAuctionDetails);
    connect(network, &NetworkManager::bidHistoryReceived, this, &MainWindow::onBidHistoryReceived);
    connect(network, &NetworkManager::auctionHistoryReceived, this, &MainWindow::onAuctionHistoryReceived);
    connect(network, &NetworkManager::notification, this, &MainWindow::onNotification);
    connect(network, &NetworkManager::newBid, this, &MainWindow::onNewBid);
    connect(network, &NetworkManager::newAuction, this, &MainWindow::onNewAuction);
    connect(network, &NetworkManager::auctionWarning, this, &MainWindow::onAuctionWarning);
    connect(network, &NetworkManager::auctionEnded, this, &MainWindow::onAuctionEnded);
    connect(network, &NetworkManager::userJoinedRoom, this, &MainWindow::onUserJoinedRoom);
    connect(network, &NetworkManager::userLeftRoom, this, &MainWindow::onUserLeftRoom);
    connect(network, &NetworkManager::roomError, this, &MainWindow::onRoomError);
    connect(network, &NetworkManager::auctionError, this, &MainWindow::onAuctionError);
    connect(network, &NetworkManager::bidError, this, &MainWindow::onBidError);
    connect(network, &NetworkManager::disconnected, this, &MainWindow::onDisconnected);

    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdowns);
    countdownTimer->start(1000);

    network->sendListRooms();

    addLogMessage("Đăng nhập thành công", "SUCCESS");
    addLogMessage(QString("Chào %1! Số dư: %2")
                      .arg(user.username)
                      .arg(Formatters::formatCurrency(user.balance)),
                  "INFO");
}

MainWindow::~MainWindow() {}

QPushButton *MainWindow::createStyledButton(const QString &text, const QString &color)
{
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(QString(
                           "QPushButton { background: %1; color: white; border: none; "
                           "padding: 10px 15px; border-radius: 6px; font-weight: bold; font-size: 13px; } "
                           "QPushButton:hover { background: %2; } "
                           "QPushButton:pressed { background: %3; }")
                           .arg(color)
                           .arg(adjustBrightness(color, 110))
                           .arg(adjustBrightness(color, 90)));
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QString MainWindow::adjustBrightness(const QString &color, int percent)
{
    QColor c(color);
    int h, s, v;
    c.getHsv(&h, &s, &v);
    v = qBound(0, v * percent / 100, 255);
    c.setHsv(h, s, v);
    return c.name();
}
void MainWindow::onAuctionDeletedBroadcast(int auctionId, QString title)
{
    Q_UNUSED(auctionId);
    addLogMessage(QString("🗑️ '%1' đã bị xóa").arg(title), "INFO");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}
void MainWindow::onRoomCreated(int roomId)
{
    Q_UNUSED(roomId);
    addLogMessage("✅ Tạo phòng thành công!", "SUCCESS");
    network->sendListRooms();
}
void MainWindow::checkAuctionWarnings()
{
    if (!currentUser.isInRoom())
        return;

    for (const Auction &a : auctions)
    {
        if (!a.isActive())
            continue;

        int timeLeft = a.getTimeLeft();

        // Warning at 30s - CHỈ 1 LẦN
        if (timeLeft <= 30 && timeLeft > 0 &&
            !warnedAuctions.contains(a.auctionId))
        {

            warnedAuctions.insert(a.auctionId);

            // POPUP NỔI BẬT
            QMessageBox *warningBox = new QMessageBox(this);
            warningBox->setWindowTitle("⚠️ CẢNH BÁO");
            warningBox->setText(QString(
                                    "<h2 style='color: #f57c00;'>⚠️ ĐẤU GIÁ SẮP KẾT THÚC!</h2>"
                                    "<p style='font-size: 16px;'><b>%1</b></p>"
                                    "<p style='font-size: 14px;'>Còn <b style='color: red;'>%2 giây</b></p>"
                                    "<p>Giá: <b>%3</b></p>")
                                    .arg(a.title)
                                    .arg(timeLeft)
                                    .arg(Formatters::formatCurrency(a.currentPrice)));
            warningBox->setIcon(QMessageBox::Warning);
            warningBox->setStyleSheet(
                "QMessageBox { background: #fff3e0; } "
                "QLabel { font-size: 14px; }");
            warningBox->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
            warningBox->show();

            // Auto close after 5s
            QTimer::singleShot(5000, warningBox, &QMessageBox::accept);

            addLogMessage(QString("⚠️ %1 còn %2s!")
                              .arg(a.title)
                              .arg(timeLeft),
                          "WARNING");
        }
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("🎪 Đấu Giá Online - " + currentUser.username);
    setMinimumSize(1400, 850);

    setStyleSheet("QMainWindow { background: #f5f7fa; }");

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // TOP BAR
    QHBoxLayout *topBar = new QHBoxLayout();

    userInfoLabel = new QLabel();
    userInfoLabel->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); "
        "color: white; padding: 15px 25px; border-radius: 10px; font-size: 15px; font-weight: bold; }");
    updateUserInfo();
    topBar->addWidget(userInfoLabel, 1);

    QPushButton *refreshAllBtn = createStyledButton("🔄 Làm mới", "#4CAF50");
    connect(refreshAllBtn, &QPushButton::clicked, [this]()
            {
        network->sendListRooms();
        if (currentUser.isInRoom()) {
            network->sendListAuctions(currentUser.currentRoomId);
        } });
    topBar->addWidget(refreshAllBtn);
    QPushButton *roomInfoBtn = createStyledButton("ℹ️ Thông tin", "#2196F3");
    connect(roomInfoBtn, &QPushButton::clicked, this, &MainWindow::on_roomInfoButton_clicked);
    topBar->addWidget(roomInfoBtn);
    QPushButton *historyBtn = createStyledButton("📜 Lịch sử", "#9C27B0");
    connect(historyBtn, &QPushButton::clicked, this, &MainWindow::on_viewHistoryButton_clicked);
    topBar->addWidget(historyBtn);

    // ← THÊM 2 BUTTONS MỚI:
    QPushButton *participatedBtn = createStyledButton("📊 Tham gia", "#9c27b0");
    connect(participatedBtn, &QPushButton::clicked,
            this, &MainWindow::on_viewParticipatedHistoryButton_clicked);
    topBar->addWidget(participatedBtn);

    QPushButton *sellerBtn = createStyledButton("👤 Làm chủ", "#ff9800");
    connect(sellerBtn, &QPushButton::clicked,
            this, &MainWindow::on_viewSellerHistoryButton_clicked);
    topBar->addWidget(sellerBtn);
    QPushButton *roomHistoryBtn = createStyledButton("🏛️ Lịch sử phòng", "#795548");
    connect(roomHistoryBtn, &QPushButton::clicked,
            this, &MainWindow::on_viewRoomHistoryButton_clicked);
    topBar->addWidget(roomHistoryBtn);

    QPushButton *logoutBtn = createStyledButton("🚪 Thoát", "#f44336");
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
    topBar->addWidget(logoutBtn);
    mainLayout->addLayout(topBar);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // LEFT - ROOMS
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);

    QLabel *roomsTitle = new QLabel("🏠 PHÒNG");
    roomsTitle->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); "
        "color: white; padding: 12px; font-size: 16px; font-weight: bold; border-radius: 8px; }");
    leftLayout->addWidget(roomsTitle);

    roomsList = new QListWidget();
    roomsList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 8px; font-size: 13px; } "
        "QListWidget::item { padding: 12px; border-bottom: 1px solid #f0f0f0; "
        "border-radius: 6px; margin: 3px; } "
        "QListWidget::item:hover { background: #e3f2fd; } "
        "QListWidget::item:selected { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e3f2fd, stop:1 #bbdefb); color: #1976d2; border: 2px solid #2196f3; font-weight: bold; }");
    connect(roomsList, &QListWidget::itemDoubleClicked, this, &MainWindow::on_roomsList_itemDoubleClicked);
    leftLayout->addWidget(roomsList, 1);

    QGridLayout *roomBtns = new QGridLayout();
    roomBtns->setSpacing(8);

    QPushButton *createRoomBtn = createStyledButton("➕ Tạo", "#2196f3");
    QPushButton *joinRoomBtn = createStyledButton("🚪 Vào", "#4caf50");
    QPushButton *leaveRoomBtn = createStyledButton("👋 Rời", "#ff9800");
    QPushButton *refreshRoomsBtn = createStyledButton("🔄 Mới", "#9e9e9e");

    connect(createRoomBtn, &QPushButton::clicked, this, &MainWindow::on_createRoomButton_clicked);
    connect(joinRoomBtn, &QPushButton::clicked, this, &MainWindow::on_joinRoomButton_clicked);
    connect(leaveRoomBtn, &QPushButton::clicked, this, &MainWindow::on_leaveRoomButton_clicked);
    connect(refreshRoomsBtn, &QPushButton::clicked, this, &MainWindow::on_refreshRoomsButton_clicked);

    roomBtns->addWidget(createRoomBtn, 0, 0);
    roomBtns->addWidget(joinRoomBtn, 0, 1);
    roomBtns->addWidget(leaveRoomBtn, 1, 0);
    roomBtns->addWidget(refreshRoomsBtn, 1, 1);
    leftLayout->addLayout(roomBtns);

    splitter->addWidget(leftPanel);

    // CENTER - AUCTIONS
    QWidget *centerPanel = new QWidget();
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPanel);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(10);

    roomStatusLabel = new QLabel("❌ Chưa vào phòng");
    roomStatusLabel->setStyleSheet(
        "QLabel { background: #ffebee; color: #c62828; padding: 12px; "
        "border-radius: 8px; font-weight: bold; font-size: 14px; }");
    centerLayout->addWidget(roomStatusLabel);

    QLabel *auctionsTitle = new QLabel("🔨 ĐẤU GIÁ");
    auctionsTitle->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f093fb, stop:1 #f5576c); "
        "color: white; padding: 12px; font-size: 16px; font-weight: bold; border-radius: 8px; }");
    centerLayout->addWidget(auctionsTitle);

    auctionsList = new QListWidget();
    auctionsList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 8px; font-size: 13px; } "
        "QListWidget::item { padding: 14px; border-bottom: 1px solid #f0f0f0; "
        "border-radius: 6px; margin: 3px; } "
        "QListWidget::item:hover { background: #fff3e0; } "
        "QListWidget::item:selected { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e8f5e9, stop:1 #c8e6c9); color: #2e7d32; border: 2px solid #4caf50; font-weight: bold; }");
    auctionsList->setSelectionMode(QAbstractItemView::SingleSelection);
    centerLayout->addWidget(auctionsList, 2);

    QLabel *queueTitle = new QLabel("⏳ HÀNG ĐỢI");
    queueTitle->setStyleSheet(
        "QLabel { background: #ff9800; color: white; padding: 8px; font-size: 14px; "
        "font-weight: bold; border-radius: 6px; }");
    centerLayout->addWidget(queueTitle);

    queueList = new QListWidget();
    queueList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 8px; background: #fff8e1; "
        "padding: 5px; font-size: 12px; max-height: 120px; } "
        "QListWidget::item { padding: 8px; border-radius: 4px; margin: 2px; }");
    centerLayout->addWidget(queueList);

    QGridLayout *auctionBtns = new QGridLayout();
    auctionBtns->setSpacing(8);
    QPushButton *createAuctionBtn = createStyledButton("➕ Tạo", "#2196f3");
    QPushButton *viewDetailsBtn = createStyledButton("👁️ Chi tiết", "#009688");
    QPushButton *activateBtn = createStyledButton("▶️ Kích hoạt", "#ff9800");

    // ✅ FIX: Gán vào member variable
    deleteAuctionButton = createStyledButton("🗑️ Xóa", "#f44336");
    deleteAuctionButton->setStyleSheet(deleteAuctionButton->styleSheet() +
                                       "QPushButton:disabled { background: #CCCCCC; color: #666666; }");

    QPushButton *searchBtn = createStyledButton("🔍 Tìm", "#9c27b0");
    QPushButton *bidHistoryBtn = createStyledButton("📊 Lịch sử giá", "#607d8b");

    connect(createAuctionBtn, &QPushButton::clicked, this, &MainWindow::on_createAuctionButton_clicked);
    connect(viewDetailsBtn, &QPushButton::clicked, this, &MainWindow::on_viewAuctionDetailsButton_clicked);
    connect(activateBtn, &QPushButton::clicked, this, &MainWindow::on_activateAuctionButton_clicked);
    connect(deleteAuctionButton, &QPushButton::clicked, this, &MainWindow::on_deleteAuctionButton_clicked);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::on_searchAuctionsButton_clicked);
    connect(bidHistoryBtn, &QPushButton::clicked, this, &MainWindow::on_bidHistoryButton_clicked);

    auctionBtns->addWidget(createAuctionBtn, 0, 0);
    auctionBtns->addWidget(viewDetailsBtn, 0, 1);
    auctionBtns->addWidget(activateBtn, 0, 2);
    auctionBtns->addWidget(deleteAuctionButton, 1, 0);
    auctionBtns->addWidget(searchBtn, 1, 1);
    auctionBtns->addWidget(bidHistoryBtn, 1, 2);
    centerLayout->addLayout(auctionBtns);

    QHBoxLayout *bidBtns = new QHBoxLayout();
    bidBtns->setSpacing(10);
    // ✅ FIX: Gán vào member variables thay vì local variables
    bidButton = new QPushButton("💰 ĐẶT GIÁ");
    bidButton->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f093fb, stop:1 #f5576c); "
        "color: white; border: none; padding: 18px; border-radius: 10px; "
        "font-size: 18px; font-weight: bold; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e084eb, stop:1 #e5475c); } "
        "QPushButton:disabled { background: #CCCCCC; color: #666666; }" // ← THÊM disabled style
    );

    buyNowButton = new QPushButton("⚡ MUA NGAY");
    buyNowButton->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4facfe, stop:1 #00f2fe); "
        "color: white; border: none; padding: 18px; border-radius: 10px; "
        "font-size: 18px; font-weight: bold; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3f9cee, stop:1 #00e2ee); } "
        "QPushButton:disabled { background: #CCCCCC; color: #666666; }" // ← THÊM disabled style
    );

    connect(bidButton, &QPushButton::clicked, this, &MainWindow::on_placeBidButton_clicked);
    connect(buyNowButton, &QPushButton::clicked, this, &MainWindow::on_buyNowButton_clicked);
    bidBtns->addWidget(bidButton);
    bidBtns->addWidget(buyNowButton);
    centerLayout->addLayout(bidBtns);

    splitter->addWidget(centerPanel);

    // RIGHT - LOG
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    QLabel *logTitle = new QLabel("📋 NHẬT KÝ");
    logTitle->setStyleSheet(
        "QLabel { background: #607d8b; color: white; padding: 12px; font-size: 16px; "
        "font-weight: bold; border-radius: 8px; }");
    rightLayout->addWidget(logTitle);

    activityLog = new QTextEdit();
    activityLog->setReadOnly(true);
    activityLog->setStyleSheet(
        "QTextEdit { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 10px; font-family: 'Segoe UI', Arial, sans-serif; font-size: 14px; }");
    rightLayout->addWidget(activityLog, 1);

    QPushButton *clearLogBtn = createStyledButton("🗑️ Xóa", "#9e9e9e");
    connect(clearLogBtn, &QPushButton::clicked, [this]()
            { activityLog->clear(); });
    rightLayout->addWidget(clearLogBtn);

    splitter->addWidget(rightPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);

    mainLayout->addWidget(splitter, 1);

    setCentralWidget(central);
    statusBar()->showMessage("✅ Kết nối");
}

void MainWindow::updateUserInfo()
{
    userInfoLabel->setText(QString("👤 %1  |  💰 %2")
                               .arg(currentUser.username)
                               .arg(Formatters::formatCurrency(currentUser.balance)));
}

void MainWindow::updateRoomStatus()
{
    if (currentUser.isInRoom())
    {
        roomStatusLabel->setText(QString("✅ Phòng: %1").arg(currentUser.currentRoomName));
        roomStatusLabel->setStyleSheet(
            "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c8e6c9, stop:1 #a5d6a7); "
            "color: #1b5e20; padding: 12px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    }
    else
    {
        roomStatusLabel->setText("❌ Chưa vào phòng");
        roomStatusLabel->setStyleSheet(
            "QLabel { background: #ffebee; color: #c62828; padding: 12px; "
            "border-radius: 8px; font-weight: bold; font-size: 14px; }");
    }
}
void MainWindow::onAuctionStarted(int auctionId)
{
    qDebug() << "[QUEUE] Auction started:" << auctionId;

    // Refresh ngay lập tức
    if (currentUser.isInRoom())
    {
        qDebug() << "[REFRESH] Auto-refreshing for new auction from queue";
        network->sendListAuctions(currentUser.currentRoomId);
    }

    addLogMessage(QString("🔨 Đấu giá mới bắt đầu!"), "INFO");
}
void MainWindow::addLogMessage(const QString &message, const QString &type)
{
    QString color, icon;
    int fontSize = 14;

    if (type == "SUCCESS")
    {
        color = "#2e7d32";
        icon = "✅";
        fontSize = 15;
    }
    else if (type == "ERROR")
    {
        color = "#c62828";
        icon = "❌";
        fontSize = 15;
    }
    else if (type == "WARNING")
    {
        color = "#f57c00";
        icon = "⚠️";
        fontSize = 15;
    }
    else if (type == "BID")
    {
        color = "#1976d2";
        icon = "💰";
        fontSize = 16;
    }
    else if (type == "WIN")
    {
        color = "#7b1fa2";
        icon = "🎉";
        fontSize = 17;
    }
    else
    {
        color = "#455a64";
        icon = "ℹ️";
        fontSize = 13;
    }

    QTextCursor cursor = activityLog->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat timeFormat;
    timeFormat.setForeground(QColor("#9e9e9e"));
    timeFormat.setFontPointSize(11);
    cursor.insertText(QString("[%1] ").arg(QTime::currentTime().toString("hh:mm:ss")), timeFormat);

    QTextCharFormat iconFormat;
    iconFormat.setFontPointSize(fontSize);
    cursor.insertText(icon + " ", iconFormat);

    QTextCharFormat msgFormat;
    msgFormat.setForeground(QColor(color));
    msgFormat.setFontPointSize(fontSize);
    if (type != "INFO")
    {
        msgFormat.setFontWeight(QFont::Bold);
    }
    cursor.insertText(message + "\n", msgFormat);

    activityLog->setTextCursor(cursor);
    activityLog->verticalScrollBar()->setValue(activityLog->verticalScrollBar()->maximum());
}

void MainWindow::showError(const QString &title, const QString &message)
{
    QMessageBox::warning(this, title, message);
    addLogMessage(message, "ERROR");
}

void MainWindow::showSuccess(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
    addLogMessage(message, "SUCCESS");
}

Room MainWindow::getSelectedRoom() const
{
    int row = roomsList->currentRow();
    return (row >= 0 && row < rooms.size()) ? rooms[row] : Room();
}
// File: windows/mainwindow.cpp
// Thay thế hàm getSelectedAuction() (dòng 490-494)
Auction MainWindow::getSelectedAuction() const
{
    QListWidgetItem *item = auctionsList->currentItem();
    if (!item)
    {
        qDebug() << "[GET AUCTION] No item selected";
        return Auction();
    }

    // Get index and text for debugging
    int row = auctionsList->row(item);
    QString itemText = item->text();

    qDebug() << "========================================";
    qDebug() << "[GET AUCTION] Selected item:";
    qDebug() << "  - Row:" << row;
    qDebug() << "  - Text:" << itemText;

    // Get auctionId from UserRole
    QVariant userData = item->data(Qt::UserRole);
    qDebug() << "  - UserRole data:" << userData;

    int auctionId = userData.toInt();
    qDebug() << "  - Parsed ID:" << auctionId;

    if (auctionId <= 0)
    {
        qDebug() << "[GET AUCTION] Invalid auction ID (probably header/separator)";
        qDebug() << "========================================";
        return Auction();
    }

    // Search in auction list
    for (const Auction &auction : auctions)
    {
        if (auction.auctionId == auctionId)
        {
            qDebug() << "[GET AUCTION] Found auction:";
            qDebug() << "  - ID:" << auction.auctionId;
            qDebug() << "  - Title:" << auction.title;
            qDebug() << "  - Status:" << auction.status;
            qDebug() << "  - Current Price:" << auction.currentPrice;
            qDebug() << "  - Buy Now:" << auction.buyNowPrice;
            qDebug() << "========================================";
            return auction;
        }
    }

    qDebug() << "[GET AUCTION] ERROR: Auction ID" << auctionId
             << "not found in local list!";
    qDebug() << "[GET AUCTION] Local list has" << auctions.size() << "auctions";
    qDebug() << "========================================";
    return Auction();
}
bool MainWindow::userHasActiveBids()
{
    for (const Auction &a : auctions)
    {
        if (a.isActive() && a.currentBidderName == currentUser.username)
        {
            return true;
        }
    }
    return false;
}

// ========== BUTTON HANDLERS ==========

void MainWindow::on_refreshRoomsButton_clicked()
{
    network->sendListRooms();
}

void MainWindow::on_createRoomButton_clicked()
{
    CreateRoomDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        network->sendCreateRoom(currentUser.userId, dlg.getRoomName(),
                                dlg.getDescription(), dlg.getMaxParticipants(), dlg.getDuration());
    }
}
void MainWindow::on_viewRoomHistoryButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Lỗi", "Vào phòng để xem lịch sử");
        return;
    }

    network->sendRoomHistory(currentUser.currentRoomId);
}
void MainWindow::onRoomHistoryReceived(const QString &history)
{
    if (history.isEmpty())
    {
        QMessageBox::information(this, "Lịch sử phòng",
                                 "Phòng chưa có đấu giá nào kết thúc");
        return;
    }

    QStringList auctions = history.split('|', Qt::SkipEmptyParts);

    qDebug() << "Room history - Total auctions:" << auctions.size();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🏛️ LỊCH SỬ PHÒNG ĐẤU GIÁ");
    dialog->setMinimumSize(1100, 650);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Title
    QLabel *title = new QLabel("🏛️ LỊCH SỬ PHÒNG ĐẤU GIÁ");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #795548; "
        "padding: 15px; background: white; border-radius: 8px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Table - 9 columns
    QTableWidget *table = new QTableWidget(auctions.size(), 10, dialog); // 9 → 10
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối",
        "👤 Người thắng", "📊 Tổng lượt", "👥 Người tham gia",
        "👨‍💼 Chủ đấu giá", "📈 Kết quả", "🎯 Phương thức" // ← THÊM CỘT MỚI
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #8d6e63, stop:1 #5d4037); color: white; padding: 10px; "
        "font-weight: bold; border: none; }");
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    double totalValue = 0;
    int totalBids = 0;
    int soldCount = 0;

    int row = 0;
    for (const QString &auctionData : auctions)
    {
        QStringList fields = auctionData.split(';');

        qDebug() << "Row" << row << "fields:" << fields.size() << fields;

        // Server format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status;seller
        if (fields.size() >= 10)
        { // 9 → 10
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int auctionTotalBids = fields[5].toInt();
            int participants = fields[6].toInt();
            QString status = fields[7];
            QString seller = fields[8];
            QString winMethod = fields[9]; // ← THÊM DÒNG MỚI

            bool sold = (winner != "No winner");
            if (sold)
            {
                soldCount++;
                totalValue += finalPrice;
            }
            totalBids += auctionTotalBids;

            // #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));

            // Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);

            // Start price
            table->setItem(row, 2, new QTableWidgetItem(Formatters::formatCurrency(startPrice)));

            // Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(sold ? QColor("#4caf50") : QColor("#666"));
            finalItem->setFont(QFont("Arial", 11, sold ? QFont::Bold : QFont::Normal));
            table->setItem(row, 3, finalItem);

            // Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(sold ? QColor("#4caf50") : QColor("#999"));
            table->setItem(row, 4, winnerItem);

            // Total bids
            QTableWidgetItem *bidsItem = new QTableWidgetItem(QString::number(auctionTotalBids));
            bidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 5, bidsItem);

            // Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(QString::number(participants));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, partItem);

            // Seller
            QTableWidgetItem *sellerItem = new QTableWidgetItem(seller);
            sellerItem->setForeground(QColor("#1976d2"));
            sellerItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 7, sellerItem);

            // Result
            QString result = sold ? "✅ Đã bán" : "❌ Chưa bán";
            QTableWidgetItem *resultItem = new QTableWidgetItem(result);
            resultItem->setFont(QFont("Arial", 11, QFont::Bold));
            resultItem->setForeground(sold ? QColor("#4caf50") : QColor("#f44336"));
            resultItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 8, resultItem);
            // Column 9: Win Method (THÊM MỚI)
            QString methodDisplay;
            QColor methodColor;
            if (status == "ended")
            {
                if (winMethod == "buy_now")
                {
                    methodDisplay = "💳 Mua ngay";
                    methodColor = QColor("#4caf50"); // Green
                }
                else if (winMethod == "bid")
                {
                    methodDisplay = "⚡ Đấu giá";
                    methodColor = QColor("#2196f3"); // Blue
                }
                else
                {
                    methodDisplay = "-";
                    methodColor = QColor("#757575"); // Gray
                }
            }
            else
            {
                methodDisplay = "⏳ Chưa kết thúc";
                methodColor = QColor("#ff9800"); // Orange
            }

            QTableWidgetItem *methodItem = new QTableWidgetItem(methodDisplay);
            methodItem->setForeground(methodColor);
            methodItem->setFont(QFont("Arial", 10, QFont::Bold));
            methodItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 9, methodItem);

            row++;
        }
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    // Stats
    QLabel *stats = new QLabel(QString(
                                   "📊 <b>Tổng:</b> %1 phiên | "
                                   "<b style='color:#4caf50;'>✅ Đã bán:</b> %2 | "
                                   "<b style='color:#f44336;'>❌ Chưa bán:</b> %3 | "
                                   "<b>Tổng lượt đặt:</b> %4 | "
                                   "<b style='color:#4caf50;'>💰 Tổng giá trị:</b> %5")
                                   .arg(auctions.size())
                                   .arg(soldCount)
                                   .arg(auctions.size() - soldCount)
                                   .arg(totalBids)
                                   .arg(Formatters::formatCurrency(totalValue)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;");
    layout->addWidget(stats);

    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #795548; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #5d4037; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    dialog->exec();
    delete dialog;
}
void MainWindow::on_joinRoomButton_clicked()
{
    Room room = getSelectedRoom();
    if (room.roomId == 0)
    {
        showError("Lỗi", "Chọn phòng");
        return;
    }

    // FIX: Check if already in a room
    if (currentUser.isInRoom())
    {
        showError("Lỗi", "Rời phòng hiện tại trước");
        return;
    }

    network->sendJoinRoom(currentUser.userId, room.roomId);
}

void MainWindow::on_leaveRoomButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Lỗi", "Chưa vào phòng");
        return;
    }

    if (userHasActiveBids())
    {
        QMessageBox::warning(this, "Không thể rời",
                             "Bạn đang có giá đặt trong đấu giá!\nĐợi khi đấu giá kết thúc.");
        addLogMessage("Không thể rời: đang có bid", "WARNING");
        return;
    }

    network->sendLeaveRoom(currentUser.userId);
}

void MainWindow::on_refreshAuctionsButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Lỗi", "Vào phòng trước");
        return;
    }
    network->sendListAuctions(currentUser.currentRoomId);
}

void MainWindow::on_createAuctionButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Lỗi", "Vào phòng trước");
        return;
    }

    CreateAuctionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        network->sendCreateAuction(currentUser.userId, currentUser.currentRoomId,
                                   dlg.getTitle(), dlg.getDescription(), dlg.getStartPrice(),
                                   dlg.getBuyNowPrice(), dlg.getMinIncrement(), dlg.getDuration());
    }
}

void MainWindow::on_viewAuctionDetailsButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0)
    {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    network->sendViewAuction(auction.auctionId);
}

void MainWindow::on_placeBidButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0)
    {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    if (!auction.canBid())
    {
        showError("Lỗi", "Không đang đấu giá");
        return;
    }

    BidPlaceDialog dlg(auction, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        double amount = dlg.getBidAmount();
        network->sendPlaceBid(auction.auctionId, currentUser.userId, amount);
    }
}
void MainWindow::on_buyNowButton_clicked()
{
    Auction auction = getSelectedAuction();

    qDebug() << "========================================";
    qDebug() << "[BUY NOW] Button clicked!";
    qDebug() << "[BUY NOW] Selected auction:" << auction.auctionId;
    qDebug() << "[BUY NOW] Title:" << auction.title;
    qDebug() << "[BUY NOW] Status:" << auction.status;
    qDebug() << "========================================";

    if (auction.auctionId == 0)
    {
        qDebug() << "[BUY NOW] ERROR: No auction selected!";
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    // ✅ CRITICAL: Double-check status before sending
    if (auction.status != "active")
    {
        qDebug() << "[BUY NOW] ERROR: Auction not active!";
        QString statusMsg;
        if (auction.status == "queued")
        {
            statusMsg = "đang trong hàng đợi, chưa bắt đầu";
        }
        else if (auction.status == "ended")
        {
            statusMsg = "đã kết thúc";
        }
        else if (auction.status == "waiting")
        {
            statusMsg = "đang chờ bắt đầu";
        }
        else
        {
            statusMsg = "không hoạt động";
        }
        showError("Lỗi", QString("Sản phẩm '%1' %2, không thể mua ngay!")
                             .arg(auction.title)
                             .arg(statusMsg));
        return;
    }

    if (!auction.hasBuyNow())
    {
        qDebug() << "[BUY NOW] ERROR: No buy now price!";
        showError("Lỗi", "Sản phẩm không có giá mua ngay");
        return;
    }

    // ✅ FINAL CONFIRMATION with all details
    auto reply = QMessageBox::question(this, "⚠️ XÁC NHẬN MUA NGAY",
                                       QString("Bạn chắc chắn muốn MUA NGAY?\n\n"
                                               "📦 Sản phẩm: %1\n"
                                               "💰 Giá: %2\n"
                                               "🆔 Mã SP: #%3\n\n"
                                               "Số dư hiện tại: %4")
                                           .arg(auction.title)
                                           .arg(Formatters::formatCurrency(auction.buyNowPrice))
                                           .arg(auction.auctionId)
                                           .arg(Formatters::formatCurrency(currentUser.balance)),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No); // Default to No for safety

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "[BUY NOW] Sending BUY_NOW command for auction" << auction.auctionId;
        network->sendBuyNow(auction.auctionId, currentUser.userId);
    }
    else
    {
        qDebug() << "[BUY NOW] User cancelled";
    }
}

void MainWindow::on_deleteAuctionButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0)
    {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       QString("Xóa %1?").arg(auction.title),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        network->sendDeleteAuction(auction.auctionId, currentUser.userId);
    }
}

void MainWindow::on_activateAuctionButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0)
    {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    network->sendActivateAuction(auction.auctionId, currentUser.userId);
}
void MainWindow::on_searchAuctionsButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Lỗi", "Vào phòng trước");
        return;
    }

    // Create search dialog
    QDialog dialog(this);
    dialog.setWindowTitle("🔍 TÌM KIẾM ĐẤU GIÁ");
    dialog.setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Keyword input
    QLabel *keywordLabel = new QLabel("Từ khóa:");
    keywordLabel->setStyleSheet("font-weight: bold;");
    QLineEdit *keywordEdit = new QLineEdit();
    keywordEdit->setPlaceholderText("Nhập tên sản phẩm...");

    // Min price input
    QLabel *minPriceLabel = new QLabel("Giá tối thiểu (VND):");
    minPriceLabel->setStyleSheet("font-weight: bold;");
    QLineEdit *minPriceEdit = new QLineEdit();
    minPriceEdit->setPlaceholderText("Để trống = không giới hạn");

    // Max price input
    QLabel *maxPriceLabel = new QLabel("Giá tối đa (VND):");
    maxPriceLabel->setStyleSheet("font-weight: bold;");
    QLineEdit *maxPriceEdit = new QLineEdit();
    maxPriceEdit->setPlaceholderText("Để trống = không giới hạn");

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *searchBtn = new QPushButton("🔍 Tìm kiếm");
    QPushButton *cancelBtn = new QPushButton("❌ Hủy");

    searchBtn->setStyleSheet(
        "QPushButton { background: #4CAF50; color: white; padding: 10px; "
        "border-radius: 5px; font-weight: bold; }"
        "QPushButton:hover { background: #45a049; }");
    cancelBtn->setStyleSheet(
        "QPushButton { background: #f44336; color: white; padding: 10px; "
        "border-radius: 5px; font-weight: bold; }"
        "QPushButton:hover { background: #da190b; }");

    buttonLayout->addWidget(searchBtn);
    buttonLayout->addWidget(cancelBtn);

    // Add to layout
    layout->addWidget(keywordLabel);
    layout->addWidget(keywordEdit);
    layout->addSpacing(10);
    layout->addWidget(minPriceLabel);
    layout->addWidget(minPriceEdit);
    layout->addSpacing(10);
    layout->addWidget(maxPriceLabel);
    layout->addWidget(maxPriceEdit);
    layout->addSpacing(20);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(searchBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Show dialog
    if (dialog.exec() == QDialog::Accepted)
    {
        QString keyword = keywordEdit->text().trimmed();

        // Parse prices
        double minPrice = -1;
        double maxPrice = -1;

        if (!minPriceEdit->text().isEmpty())
        {
            minPrice = minPriceEdit->text().toDouble();
        }
        if (!maxPriceEdit->text().isEmpty())
        {
            maxPrice = maxPriceEdit->text().toDouble();
        }

        // Send search request
        network->sendSearchAuctions(currentUser.currentRoomId, keyword, minPrice, maxPrice);

        // Log
        QString logMsg = QString("🔍 Tìm: \"%1\"").arg(keyword.isEmpty() ? "tất cả" : keyword);
        if (minPrice > 0 || maxPrice > 0)
        {
            logMsg += QString(" [Giá: %1-%2]")
                          .arg(minPrice > 0 ? QString::number((int)minPrice) : "∞")
                          .arg(maxPrice > 0 ? QString::number((int)maxPrice) : "∞");
        }
        addLogMessage(logMsg, "INFO");
    }
}

void MainWindow::on_bidHistoryButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0)
    {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }

    network->sendBidHistory(auction.auctionId);
}

void MainWindow::on_viewHistoryButton_clicked()
{
    network->sendAuctionHistory(currentUser.userId);
}

void MainWindow::on_roomInfoButton_clicked()
{
    if (!currentUser.isInRoom())
    {
        showError("Thông tin", "Chưa vào phòng");
        return;
    }

    Room currentRoom;
    for (const Room &r : rooms)
    {
        if (r.roomId == currentUser.currentRoomId)
        {
            currentRoom = r;
            break;
        }
    }

    QString info = QString(
                       "🏠 PHÒNG: %1\n\n"
                       "👤 Người tạo: %2\n"
                       "👥 Người: %3/%4\n"
                       "🔨 Đấu giá: %5 active + %6 chờ\n"
                       "📊 Bid của bạn: %7")
                       .arg(currentRoom.name.isEmpty() ? currentUser.currentRoomName : currentRoom.name)
                       .arg(currentRoom.creatorName.isEmpty() ? "N/A" : currentRoom.creatorName)
                       .arg(currentRoom.currentParticipants)
                       .arg(currentRoom.maxParticipants)
                       .arg(auctions.size())
                       .arg(queueList->count())
                       .arg(userHasActiveBids() ? "CÓ ⚠️" : "Không");

    QMessageBox::information(this, "Thông tin phòng", info);
}

void MainWindow::on_logoutButton_clicked()
{
    if (userHasActiveBids())
    {
        auto reply = QMessageBox::question(this, "Xác nhận",
                                           "Bạn đang có bid!\nVẫn đăng xuất?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }

    network->sendLogout(currentUser.userId);
    close();
}

void MainWindow::on_roomsList_itemDoubleClicked(QListWidgetItem *)
{
    on_joinRoomButton_clicked();
}

void MainWindow::on_roomsList_itemSelectionChanged() {}
void MainWindow::on_auctionsList_itemSelectionChanged() {}

// ========== NETWORK HANDLERS ==========

void MainWindow::onRoomListReceived(const QList<Room> &newRooms)
{
    rooms = newRooms;
    roomsList->clear();

    for (const Room &room : rooms)
    {
        QString icon = room.isFull() ? "🔒" : "🏠";
        QString text = QString("%1 %2 | 👥 %3/%4")
                           .arg(icon)
                           .arg(room.name)
                           .arg(room.currentParticipants)
                           .arg(room.maxParticipants);

        QListWidgetItem *item = new QListWidgetItem(text);
        if (room.isFull())
        {
            item->setForeground(QColor("#9e9e9e"));
        }
        roomsList->addItem(item);
    }
}

void MainWindow::onJoinedRoom(int roomId, const QString &roomName)
{
    currentUser.currentRoomId = roomId;
    currentUser.currentRoomName = roomName;
    updateRoomStatus();
    addLogMessage(QString("Vào phòng: %1").arg(roomName), "SUCCESS");

    network->sendListAuctions(roomId);
}

void MainWindow::onLeftRoom()
{
    QString oldRoom = currentUser.currentRoomName;
    currentUser.currentRoomId = 0;
    currentUser.currentRoomName.clear();
    updateRoomStatus();
    auctionsList->clear();
    queueList->clear();
    auctions.clear();
    addLogMessage(QString("Rời phòng: %1").arg(oldRoom), "INFO");
}
void MainWindow::updateAuctionActionButtons()
{
    Auction auction = getSelectedAuction();

    qDebug() << "========================================";
    qDebug() << "[UI DEBUG] Selected Auction:";
    qDebug() << "  - ID:" << auction.auctionId;
    qDebug() << "  - Title:" << auction.title;
    qDebug() << "  - Status:" << auction.status;
    qDebug() << "  - Seller Name:" << auction.sellerName;
    qDebug() << "  - Seller ID:" << auction.sellerId;
    qDebug() << "  - Current User:" << currentUser.username;
    qDebug() << "  - Current User ID:" << currentUser.userId;
    qDebug() << "  - Current Price:" << auction.currentPrice;
    qDebug() << "  - Buy Now Price:" << auction.buyNowPrice;
    qDebug() << "  - Total Bids:" << auction.totalBids;
    qDebug() << "========================================";

    bool hasValidAuction = (auction.auctionId > 0);
    bool isActive = (hasValidAuction && auction.status == "active");

    // ═══════════════════════════════════════════════════════
    // BID BUTTON
    // ═══════════════════════════════════════════════════════
    if (bidButton)
    {
        // Chỉ enable khi:
        // 1. Auction đang active
        // 2. User KHÔNG phải seller (không đấu giá sản phẩm của mình)
        bool isSeller = (auction.sellerName == currentUser.username ||
                         auction.sellerId == currentUser.userId);
        bool canBid = isActive && !isSeller;

        bidButton->setEnabled(canBid);

        if (!hasValidAuction)
        {
            bidButton->setToolTip("Chọn một sản phẩm để đấu giá");
        }
        else if (!isActive)
        {
            bidButton->setToolTip("Chỉ có thể đấu giá khi sản phẩm đang active");
        }
        else if (isSeller)
        {
            bidButton->setToolTip("Không thể đấu giá sản phẩm của chính mình");
        }
        else
        {
            bidButton->setToolTip(QString("Đặt giá tối thiểu: %1")
                                      .arg(Formatters::formatCurrency(auction.currentPrice + auction.minIncrement)));
        }

        qDebug() << "[UI] Bid button:";
        qDebug() << "  - enabled:" << canBid;
        qDebug() << "  - isActive:" << isActive;
        qDebug() << "  - isSeller:" << isSeller;
    }

    // ═══════════════════════════════════════════════════════
    // BUY NOW BUTTON
    // ═══════════════════════════════════════════════════════
    if (buyNowButton)
    {
        // Chỉ enable khi:
        // 1. Auction đang active
        // 2. Có giá mua ngay
        // 3. User KHÔNG phải seller
        bool isSeller = (auction.sellerName == currentUser.username ||
                         auction.sellerId == currentUser.userId);
        bool canBuyNow = isActive && auction.hasBuyNow() && !isSeller;

        buyNowButton->setEnabled(canBuyNow);

        if (!hasValidAuction)
        {
            buyNowButton->setToolTip("Chọn một sản phẩm để mua ngay");
        }
        else if (!isActive)
        {
            buyNowButton->setToolTip("Sản phẩm không đang active");
        }
        else if (!auction.hasBuyNow())
        {
            buyNowButton->setToolTip("Sản phẩm không có giá mua ngay");
        }
        else if (isSeller)
        {
            buyNowButton->setToolTip("Không thể mua sản phẩm của chính mình");
        }
        else
        {
            buyNowButton->setToolTip(QString("Mua ngay với %1")
                                         .arg(Formatters::formatCurrency(auction.buyNowPrice)));
        }

        qDebug() << "[UI] Buy Now button:";
        qDebug() << "  - enabled:" << canBuyNow;
        qDebug() << "  - isActive:" << isActive;
        qDebug() << "  - hasBuyNow:" << auction.hasBuyNow();
        qDebug() << "  - isSeller:" << isSeller;
    }

    // ═══════════════════════════════════════════════════════
    // DELETE BUTTON
    // ═══════════════════════════════════════════════════════
    if (deleteAuctionButton)
    {
        // Có thể xóa khi:
        // 1. User là seller (chủ sản phẩm)
        // 2. Auction chưa bắt đầu (queued/waiting) HOẶC
        // 3. Auction đang active NHƯNG chưa có ai đấu giá

        bool isSeller = hasValidAuction &&
                        (auction.sellerName == currentUser.username ||
                         auction.sellerId == currentUser.userId);

        bool canDeleteStatus = (auction.status == "queued" ||
                                auction.status == "waiting" ||
                                (auction.status == "active" && auction.totalBids == 0));

        bool canDelete = isSeller && canDeleteStatus;

        deleteAuctionButton->setEnabled(canDelete);

        // Tooltip chi tiết
        if (!hasValidAuction)
        {
            deleteAuctionButton->setToolTip("Chọn một sản phẩm để xóa");
        }
        else if (!isSeller)
        {
            deleteAuctionButton->setToolTip("Chỉ chủ sản phẩm mới có thể xóa");
        }
        else if (!canDeleteStatus)
        {
            if (auction.status == "active" && auction.totalBids > 0)
            {
                deleteAuctionButton->setToolTip(QString("Không thể xóa - đã có %1 người đấu giá")
                                                    .arg(auction.totalBids));
            }
            else if (auction.status == "ended")
            {
                deleteAuctionButton->setToolTip("Không thể xóa - đấu giá đã kết thúc");
            }
            else
            {
                deleteAuctionButton->setToolTip("Không thể xóa ở trạng thái này");
            }
        }
        else
        {
            deleteAuctionButton->setToolTip("Xóa sản phẩm");
        }

        qDebug() << "[UI] Delete button:";
        qDebug() << "  - enabled:" << canDelete;
        qDebug() << "  - hasValidAuction:" << hasValidAuction;
        qDebug() << "  - isSeller:" << isSeller;
        qDebug() << "  - auction.sellerName:" << auction.sellerName;
        qDebug() << "  - currentUser.username:" << currentUser.username;
        qDebug() << "  - auction.sellerId:" << auction.sellerId;
        qDebug() << "  - currentUser.userId:" << currentUser.userId;
        qDebug() << "  - canDeleteStatus:" << canDeleteStatus;
        qDebug() << "  - auction.status:" << auction.status;
        qDebug() << "  - auction.totalBids:" << auction.totalBids;
    }

    qDebug() << "========================================";
}
void MainWindow::onAuctionListReceived(const QList<Auction> &auctions)
{
    this->auctions = auctions;
    auctionsList->clear();

    qDebug() << "========================================";
    qDebug() << "[AUCTION LIST] Received" << auctions.size() << "auctions";

    int activeCount = 0;
    int queuedCount = 0;
    int endedCount = 0;

    // Debug: Log all received auctions
    for (const Auction &auction : auctions)
    {
        qDebug() << "[AUCTION LIST]  -" << auction.auctionId << ":"
                 << auction.title << "| Status:" << auction.status;
    }

    // === SECTION 1: ACTIVE AUCTIONS ===
    bool hasActive = false;
    for (const Auction &auction : auctions)
    {
        if (auction.status == "active")
        {
            hasActive = true;
            break;
        }
    }

    QListWidgetItem *firstActiveItem = nullptr; // Track first active item
    QListWidgetItem *firstAnyItem = nullptr;    // Track first item (any status)

    if (hasActive)
    {
        // Add header
        QListWidgetItem *activeHeader = new QListWidgetItem("━━━ 🔨 ĐANG ĐẤU GIÁ ━━━");
        QFont headerFont = activeHeader->font();
        headerFont.setBold(true);
        activeHeader->setFont(headerFont);
        activeHeader->setForeground(QColor("#4CAF50"));
        activeHeader->setFlags(Qt::ItemIsEnabled); // Not selectable
        activeHeader->setData(Qt::UserRole, 0);
        auctionsList->addItem(activeHeader);

        // Add active auctions
        for (const Auction &auction : auctions)
        {
            if (auction.status != "active")
                continue;

            QString statusText = Formatters::formatTime(auction.getTimeLeft());
            QString text = QString("   🔨 %1 | 💰 %2 | ⏱️ %3")
                               .arg(auction.title)
                               .arg(Formatters::formatCurrency(auction.currentPrice))
                               .arg(statusText);

            QListWidgetItem *item = new QListWidgetItem(text);
            item->setData(Qt::UserRole, auction.auctionId);
            item->setForeground(QColor("#4CAF50"));

            // ✅ Tooltip with full info
            QString tooltip = QString("🆔 ID: %1\n"
                                      "📦 Sản phẩm: %2\n"
                                      "💰 Giá hiện tại: %3\n"
                                      "⚡ Mua ngay: %4\n"
                                      "⏱️ Thời gian còn lại: %5\n"
                                      "🔨 Tổng lượt đấu: %6")
                                  .arg(auction.auctionId)
                                  .arg(auction.title)
                                  .arg(Formatters::formatCurrency(auction.currentPrice))
                                  .arg(auction.hasBuyNow() ? Formatters::formatCurrency(auction.buyNowPrice) : "Không có")
                                  .arg(statusText)
                                  .arg(auction.totalBids);
            item->setToolTip(tooltip);

            auctionsList->addItem(item);
            activeCount++;

            if (!firstActiveItem)
            {
                firstActiveItem = item;
            }
            // ✅ Track first item of any status
            if (!firstAnyItem)
            {
                firstAnyItem = item;
            }

            qDebug() << "[AUCTION LIST]    + Added active:" << auction.auctionId
                     << auction.title;
        }
    }

    // === SECTION 2: QUEUED AUCTIONS ===
    bool hasQueued = false;
    for (const Auction &auction : auctions)
    {
        if (auction.status == "queued")
        {
            hasQueued = true;
            break;
        }
    }

    if (hasQueued)
    {
        // Add separator (if there were active auctions)
        if (hasActive)
        {
            QListWidgetItem *separator = new QListWidgetItem(" ");
            separator->setFlags(Qt::ItemIsEnabled);
            separator->setData(Qt::UserRole, 0);
            auctionsList->addItem(separator);
        }

        // Add header
        QListWidgetItem *queueHeader = new QListWidgetItem("━━━ 📋 HÀNG ĐỢI ━━━");
        QFont headerFont = queueHeader->font();
        headerFont.setBold(true);
        queueHeader->setFont(headerFont);
        queueHeader->setForeground(QColor("#2196F3"));
        queueHeader->setFlags(Qt::ItemIsEnabled);
        queueHeader->setData(Qt::UserRole, 0);
        auctionsList->addItem(queueHeader);

        // Add queued auctions (sorted by position)
        QList<Auction> queuedList;
        for (const Auction &auction : auctions)
        {
            if (auction.status == "queued")
            {
                queuedList.append(auction);
            }
        }

        // Sort by queue position
        std::sort(queuedList.begin(), queuedList.end(),
                  [](const Auction &a, const Auction &b)
                  {
                      return a.queuePosition < b.queuePosition;
                  });

        for (const Auction &auction : queuedList)
        {
            QString text = QString("   📋 #%1 - %2 | 💰 %3")
                               .arg(auction.queuePosition)
                               .arg(auction.title)
                               .arg(Formatters::formatCurrency(auction.startPrice));

            QListWidgetItem *item = new QListWidgetItem(text);
            item->setData(Qt::UserRole, auction.auctionId);
            item->setForeground(QColor("#2196F3"));

            // ✅ Tooltip for queued auctions
            QString tooltip = QString("🆔 ID: %1\n"
                                      "📦 Sản phẩm: %2\n"
                                      "📋 Vị trí trong hàng đợi: #%3\n"
                                      "💰 Giá khởi điểm: %4\n"
                                      "⚡ Mua ngay: %5\n"
                                      "⚠️ Trạng thái: Đang chờ đấu giá")
                                  .arg(auction.auctionId)
                                  .arg(auction.title)
                                  .arg(auction.queuePosition)
                                  .arg(Formatters::formatCurrency(auction.startPrice))
                                  .arg(auction.hasBuyNow() ? Formatters::formatCurrency(auction.buyNowPrice) : "Không có");
            item->setToolTip(tooltip);

            auctionsList->addItem(item);
            queuedCount++;

            qDebug() << "[AUCTION LIST]    + Added queued:" << auction.auctionId
                     << auction.title << "at position" << auction.queuePosition;
        }
    }

    // === SECTION 3: ENDED AUCTIONS ===
    for (const Auction &auction : auctions)
    {
        if (auction.status == "ended")
        {
            endedCount++;
        }
    }

    if (endedCount > 0)
    {
        if (hasActive || hasQueued)
        {
            QListWidgetItem *separator = new QListWidgetItem(" ");
            separator->setFlags(Qt::ItemIsEnabled);
            separator->setData(Qt::UserRole, 0);
            auctionsList->addItem(separator);
        }

        QListWidgetItem *endedInfo = new QListWidgetItem(
            QString("━━━ ✅ ĐÃ KẾT THÚC: %1 sản phẩm ━━━").arg(endedCount));
        QFont infoFont = endedInfo->font();
        infoFont.setItalic(true);
        endedInfo->setFont(infoFont);
        endedInfo->setForeground(QColor("#999999"));
        endedInfo->setFlags(Qt::ItemIsEnabled);
        endedInfo->setData(Qt::UserRole, 0);
        endedInfo->setToolTip("Các sản phẩm đã kết thúc không hiển thị để giữ giao diện gọn");
        auctionsList->addItem(endedInfo);
    }

    // === SUMMARY ===
    qDebug() << "[AUCTION LIST] Summary:";
    qDebug() << "[AUCTION LIST]   - Active:" << activeCount;
    qDebug() << "[AUCTION LIST]   - Queued:" << queuedCount;
    qDebug() << "[AUCTION LIST]   - Ended:" << endedCount;
    qDebug() << "========================================";

    // Empty state
    if (activeCount == 0 && queuedCount == 0)
    {
        QListWidgetItem *emptyItem = new QListWidgetItem("📭 Không có phiên đấu giá nào");
        emptyItem->setFlags(Qt::ItemIsEnabled);
        emptyItem->setData(Qt::UserRole, 0);
        emptyItem->setForeground(QColor("#999999"));
        QFont emptyFont = emptyItem->font();
        emptyFont.setItalic(true);
        emptyItem->setFont(emptyFont);
        auctionsList->addItem(emptyItem);
    }

    // ✅ Auto-select: Ưu tiên active, nếu không có thì select bất kỳ
    if (firstActiveItem)
    {
        auctionsList->setCurrentItem(firstActiveItem);
        qDebug() << "[AUCTION LIST] ✅ Auto-selected first active auction";
    }
    else if (firstAnyItem)
    {
        auctionsList->setCurrentItem(firstAnyItem);
        qDebug() << "[AUCTION LIST] ✅ Auto-selected first available auction (ended)";
    }
    else
    {
        // Chỉ clear nếu HOÀN TOÀN không có auction
        auctionsList->clearSelection();
        qDebug() << "[AUCTION LIST] No auctions at all, cleared selection";
    }

    // ✅ Update button states AFTER selection
    updateAuctionActionButtons();
}
void MainWindow::onSearchResultsReceived(const QString &results)
{
    qDebug() << "========================================";
    qDebug() << "[MAINWINDOW] onSearchResultsReceived CALLED!";
    qDebug() << "[MAINWINDOW] Results:" << results;
    qDebug() << "[MAINWINDOW] Results length:" << results.length();
    qDebug() << "========================================";

    if (results.isEmpty())
    {
        qDebug() << "[MAINWINDOW] Results is EMPTY!";
        QMessageBox::information(this, "Kết quả tìm kiếm",
                                 "❌ Không tìm thấy sản phẩm nào phù hợp");
        return;
    }

    QStringList auctions = results.split('|', Qt::SkipEmptyParts);
    qDebug() << "[MAINWINDOW] Number of auctions:" << auctions.size();

    // Create results dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🔍 KẾT QUẢ TÌM KIẾM");
    dialog->setMinimumSize(1000, 500);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Title
    QLabel *title = new QLabel(QString("✅ Tìm thấy %1 sản phẩm").arg(auctions.size()));
    title->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #4CAF50; "
        "padding: 15px; background: white; border-radius: 8px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Table
    QTableWidget *table = new QTableWidget(auctions.size(), 8, dialog);
    table->setHorizontalHeaderLabels({"#", "🏷️ Sản phẩm", "💵 Giá hiện tại", "💰 Mua ngay",
                                      "⏰ Thời gian", "📊 Lượt đấu", "📍 Trạng thái", "👤 Người bán"});

    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #2196F3, stop:1 #1976D2); color: white; padding: 10px; "
        "font-weight: bold; border: none; }");

    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    // Fill table
    int row = 0;
    for (const QString &auctionData : auctions)
    {
        QStringList fields = auctionData.split(';');

        qDebug() << "[MAINWINDOW] Auction fields:" << fields;

        // Server format: id;title;currentPrice;buyNow;minInc;timeLeft;totalBids;status;sellerId;sellerName
        if (fields.size() >= 10)
        {
            int auctionId = fields[0].toInt();
            QString title = fields[1];
            double currentPrice = fields[2].toDouble();
            double buyNowPrice = fields[3].toDouble();
            double minIncrement = fields[4].toDouble(); // ← Đã có nhưng không dùng
            int timeLeft = fields[5].toInt();
            int totalBids = fields[6].toInt();
            QString status = fields[7];
            int sellerId = fields[8].toInt();
            QString sellerName = fields[9];

            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));

            QTableWidgetItem *titleItem = new QTableWidgetItem(title);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);

            table->setItem(row, 2, new QTableWidgetItem(Formatters::formatCurrency(currentPrice)));

            QString buyNowText = (buyNowPrice > 0) ? Formatters::formatCurrency(buyNowPrice) : "-";
            table->setItem(row, 3, new QTableWidgetItem(buyNowText));

            QString timeText;
            if (status == "active")
            {
                int hours = timeLeft / 3600;
                int minutes = (timeLeft % 3600) / 60;
                int secs = timeLeft % 60;
                timeText = QString("%1h %2m %3s").arg(hours).arg(minutes).arg(secs);
            }
            else
            {
                timeText = "Đã kết thúc";
            }
            table->setItem(row, 4, new QTableWidgetItem(timeText));

            table->setItem(row, 5, new QTableWidgetItem(QString::number(totalBids)));

            QString statusText = (status == "active") ? "🟢 Đang diễn ra" : "⚫ Đã kết thúc";
            QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
            statusItem->setForeground(status == "active" ? QColor("#4CAF50") : QColor("#757575"));
            table->setItem(row, 6, statusItem);

            // ✅ Dùng sellerName thay vì fields[7]
            table->setItem(row, 7, new QTableWidgetItem(sellerName));

            row++;
        }
    }

    table->resizeColumnsToContents();
    table->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(table);

    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #2196F3; color: white; padding: 10px; "
        "border-radius: 5px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background: #1976D2; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    qDebug() << "[MAINWINDOW] Showing search results dialog";
    dialog->exec();
}
void MainWindow::onAuctionCreated(int auctionId)
{
    Q_UNUSED(auctionId);
    addLogMessage("Tạo đấu giá OK", "SUCCESS");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionActivated()
{
    addLogMessage("Kích hoạt OK", "SUCCESS");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onBidPlaced()
{
    addLogMessage("Đặt giá thành công", "SUCCESS");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onBuyNowSuccess()
{
    addLogMessage("Mua ngay thành công", "WIN");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionDeleted()
{
    addLogMessage("Đã xóa", "INFO");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionDetails(const Auction &auction)
{
    QString details = QString(
                          "🏷️ %1\n\n"
                          "💵 Giá: %2\n"
                          "⚡ Mua ngay: %3\n"
                          "👤 Cao nhất: %4\n"
                          "📊 Lượt: %5\n"
                          "⏰ Trạng thái: %6\n"
                          "⏱️ Còn: %7")
                          .arg(auction.title)
                          .arg(Formatters::formatCurrency(auction.currentPrice))
                          .arg(auction.hasBuyNow() ? Formatters::formatCurrency(auction.buyNowPrice) : "Không")
                          .arg(auction.currentBidderName.isEmpty() ? "Chưa có" : auction.currentBidderName)
                          .arg(auction.totalBids)
                          .arg(auction.getStatusText())
                          .arg(auction.isActive() ? Formatters::formatTime(auction.getTimeLeft()) : "N/A");

    QMessageBox::information(this, "Chi tiết", details);
}

void MainWindow::onBidHistoryReceived(const QString &history)
{
    QMessageBox::information(this, "Lịch sử giá", history.isEmpty() ? "Chưa có bid" : history);
} // ==================== LỊCH SỬ THAM GIA ====================
void MainWindow::on_viewParticipatedHistoryButton_clicked()
{
    network->sendAuctionHistory(currentUser.userId); // Existing
}
void MainWindow::onAuctionHistoryReceived(const QString &history)
{
    if (history.isEmpty())
    {
        QMessageBox::information(this, "Lịch sử tham gia",
                                 "Bạn chưa tham gia đấu giá nào");
        return;
    }

    QStringList auctions = history.split('|', Qt::SkipEmptyParts);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 LỊCH SỬ THAM GIA ĐẤU GIÁ");
    dialog->setMinimumSize(1100, 650);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Title
    QLabel *title = new QLabel("📊 CÁC PHIÊN ĐẤU GIÁ BẠN ĐÃ THAM GIA");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #9c27b0; "
        "padding: 15px; background: white; border-radius: 8px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Table
    QTableWidget *table = new QTableWidget(auctions.size(), 10, dialog); // 9 → 10
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối",
        "👤 Người thắng", "🎯 Lượt của bạn", "📊 Tổng lượt",
        "👥 Người tham gia", "🏆 Kết quả", "🎯 Phương thức" // ← THÊM CỘT MỚI
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #ab47bc, stop:1 #7b1fa2); color: white; padding: 10px; "
        "font-weight: bold; border: none; }");
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    int wonCount = 0, lostCount = 0;
    double totalSpent = 0;
    int totalBidsPlaced = 0;

    int row = 0;
    for (const QString &auctionData : auctions)
    {
        QStringList fields = auctionData.split(';');

        // Server format: auctionId;title;startPrice;finalPrice;winner;userBidCount;totalBids;participants;status
        if (fields.size() >= 10)
        { // 9 → 10
            // int auctionId = fields[0].toInt();
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int userBidCount = fields[5].toInt();
            int totalBids = fields[6].toInt();
            int participantCount = fields[7].toInt();
            QString status = fields[8];
            QString winMethod = fields[9]; // ← THÊM DÒNG MỚI
                                           // Check if auction actually sold
            bool sold = false;
            if (status == "ended")
            {
                if (winMethod == "buy_now" || winMethod == "bid")
                {
                    sold = true;
                }
                else if (winner != "No winner" && !winner.isEmpty())
                {
                    sold = true;
                }
            }

            // Only count if auction was sold
            bool userWon = sold && (winner == currentUser.username);
            if (userWon)
            {
                wonCount++;
                totalSpent += finalPrice;
            }
            else if (sold)
            { // ← CHỈ count loss nếu auction thực sự bán cho người khác
                lostCount++;
            }
            totalBidsPlaced += userBidCount;

            // #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));

            // Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);

            // Start price
            table->setItem(row, 2, new QTableWidgetItem(Formatters::formatCurrency(startPrice)));

            // Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(QColor("#4caf50"));
            finalItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 3, finalItem);

            // Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(userWon ? QColor("#4caf50") : QColor("#666"));
            winnerItem->setFont(QFont("Arial", 11, userWon ? QFont::Bold : QFont::Normal));
            table->setItem(row, 4, winnerItem);

            // User bid count
            QTableWidgetItem *userBidsItem = new QTableWidgetItem(
                QString::number(userBidCount));
            userBidsItem->setTextAlignment(Qt::AlignCenter);
            userBidsItem->setFont(QFont("Arial", 11, QFont::Bold));
            userBidsItem->setForeground(QColor("#1976d2"));
            table->setItem(row, 5, userBidsItem);

            // Total bids
            QTableWidgetItem *totalBidsItem = new QTableWidgetItem(
                QString::number(totalBids));
            totalBidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, totalBidsItem);

            // Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(
                QString::number(participantCount));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 7, partItem);

            // Result
            QString result = userWon ? "🎉 THẮNG" : "😢 THUA";
            QTableWidgetItem *resultItem = new QTableWidgetItem(result);
            resultItem->setFont(QFont("Arial", 12, QFont::Bold));
            resultItem->setForeground(userWon ? QColor("#4caf50") : QColor("#f57c00"));
            resultItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 8, resultItem);

            // Column 9: Win Method (THÊM MỚI)
            QString methodDisplay;
            QColor methodColor;
            if (status == "ended")
            {
                if (winMethod == "buy_now")
                {
                    methodDisplay = "💳 Mua ngay";
                    methodColor = QColor("#4caf50"); // Green
                }
                else if (winMethod == "bid")
                {
                    methodDisplay = "⚡ Đấu giá";
                    methodColor = QColor("#2196f3"); // Blue
                }
                else
                {
                    methodDisplay = "-";
                    methodColor = QColor("#757575"); // Gray
                }
            }
            else
            {
                methodDisplay = "⏳ Chưa kết thúc";
                methodColor = QColor("#ff9800"); // Orange
            }

            QTableWidgetItem *methodItem = new QTableWidgetItem(methodDisplay);
            methodItem->setForeground(methodColor);
            methodItem->setFont(QFont("Arial", 10, QFont::Bold));
            methodItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 9, methodItem);

            row++;
        }
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    // Stats
    QLabel *stats = new QLabel(QString(
                                   "📊 <b>Tổng:</b> %1 phiên | "
                                   "<b style='color:#4caf50;'>🎉 Thắng:</b> %2 | "
                                   "<b style='color:#f57c00;'>😢 Thua:</b> %3 | "
                                   "<b>Tỷ lệ:</b> %4%% | "
                                   "<b>Tổng lượt đặt:</b> %5 | "
                                   "<b style='color:#e91e63;'>💰 Tổng chi:</b> %6")
                                   .arg(auctions.size())
                                   .arg(wonCount)
                                   .arg(lostCount)
                                   .arg(auctions.size() > 0 ? wonCount * 100 / auctions.size() : 0)
                                   .arg(totalBidsPlaced)
                                   .arg(Formatters::formatCurrency(totalSpent)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;");
    layout->addWidget(stats);

    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #9c27b0; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #7b1fa2; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    dialog->exec();
    delete dialog;
}
// ==================== LỊCH SỬ LÀM CHỦ ====================
void MainWindow::on_viewSellerHistoryButton_clicked()
{
    // Request seller history
    network->sendSellerHistory(currentUser.userId);
}
void MainWindow::onBalanceUpdated(double newBalance)
{
    currentUser.balance = newBalance;
    updateUserInfo();
}
void MainWindow::onSellerHistoryReceived(const QString &history)
{
    if (history.isEmpty())
    {
        QMessageBox::information(this, "Lịch sử làm chủ",
                                 "Bạn chưa tạo đấu giá nào");
        return;
    }

    QStringList auctions = history.split('|', Qt::SkipEmptyParts);

    qDebug() << "Seller history - Total auctions:" << auctions.size();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("👤 LỊCH SỬ LÀM CHỦ ĐẤU GIÁ");
    dialog->setMinimumSize(1000, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Title
    QLabel *title = new QLabel("👤 CÁC PHIÊN ĐẤU GIÁ BẠN ĐÃ TẠO");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #ff9800; "
        "padding: 15px; background: white; border-radius: 8px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    QTableWidget *table = new QTableWidget(auctions.size(), 9, dialog); // 8 → 9
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối",
        "👤 Người thắng", "📊 Tổng lượt", "👥 Người tham gia", "📈 Trạng thái",
        "🎯 Phương thức" // ← THÊM CỘT MỚI
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #ff9800, stop:1 #f57c00); color: white; padding: 10px; "
        "font-weight: bold; border: none; }");
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    double totalRevenue = 0;
    int totalBids = 0;
    int soldCount = 0;

    int row = 0;
    for (const QString &auctionData : auctions)
    {
        QStringList fields = auctionData.split(';');

        qDebug() << "Row" << row << "fields:" << fields.size() << fields;

        // Server format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status
        if (fields.size() >= 9)
        { // 8 → 9
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int auctionTotalBids = fields[5].toInt();
            int participants = fields[6].toInt();
            QString status = fields[7];
            QString winMethod = fields[8]; // ← THÊM DÒNG MỚI

            bool sold = (winner != "No winner");
            if (sold)
            {
                soldCount++;
                totalRevenue += finalPrice;
            }
            totalBids += auctionTotalBids;

            // Column 0: #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));

            // Column 1: Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);

            // Column 2: Start price
            table->setItem(row, 2, new QTableWidgetItem(Formatters::formatCurrency(startPrice)));

            // Column 3: Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(sold ? QColor("#4caf50") : QColor("#666"));
            finalItem->setFont(QFont("Arial", 11, sold ? QFont::Bold : QFont::Normal));
            table->setItem(row, 3, finalItem);

            // Column 4: Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(sold ? QColor("#4caf50") : QColor("#999"));
            table->setItem(row, 4, winnerItem);

            // Column 5: Total bids
            QTableWidgetItem *bidsItem = new QTableWidgetItem(QString::number(auctionTotalBids));
            bidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 5, bidsItem);

            // Column 6: Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(QString::number(participants));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, partItem);

            // Column 7: Status
            QTableWidgetItem *statusItem = new QTableWidgetItem(sold ? "✅ Đã bán" : "❌ Chưa bán");
            statusItem->setForeground(sold ? QColor("#4caf50") : QColor("#f44336"));
            statusItem->setFont(QFont("Arial", 11, QFont::Bold));
            statusItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 7, statusItem);

            QString methodDisplay;
            QColor methodColor;
            if (status == "ended")
            {
                if (winMethod == "buy_now")
                {
                    methodDisplay = "💳 Mua ngay";
                    methodColor = QColor("#4caf50"); // Green
                }
                else if (winMethod == "bid")
                {
                    methodDisplay = "⚡ Đấu giá";
                    methodColor = QColor("#2196f3"); // Blue
                }
                else
                {
                    methodDisplay = "-";
                    methodColor = QColor("#757575"); // Gray
                }
            }
            else
            {
                methodDisplay = "⏳ Chưa kết thúc";
                methodColor = QColor("#ff9800"); // Orange
            }

            QTableWidgetItem *methodItem = new QTableWidgetItem(methodDisplay);
            methodItem->setForeground(methodColor);
            methodItem->setFont(QFont("Arial", 10, QFont::Bold));
            methodItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 8, methodItem);
            row++;
        }
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    // Stats
    QLabel *stats = new QLabel(QString(
                                   "📊 <b>Tổng:</b> %1 phiên | "
                                   "<b style='color:#4caf50;'>✅ Đã bán:</b> %2 | "
                                   "<b style='color:#f44336;'>❌ Chưa bán:</b> %3 | "
                                   "<b>Tổng lượt đặt:</b> %4 | "
                                   "<b style='color:#4caf50;'>💰 Doanh thu:</b> %5")
                                   .arg(auctions.size())
                                   .arg(soldCount)
                                   .arg(auctions.size() - soldCount)
                                   .arg(totalBids)
                                   .arg(Formatters::formatCurrency(totalRevenue)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;");
    layout->addWidget(stats);

    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #ff9800; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #f57c00; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    dialog->exec();
    delete dialog;
}
void MainWindow::onNotification(const QString &type, const QString &data)
{
    QStringList parts = data.split("|");

    // ... existing notification handlers ...

    if (type == "NOTIF_AUCTION_QUEUED")
    {
        // Format: auctionId|title|seller|position
        QString title = parts.value(1);
        int position = parts.value(3).toInt();

        addLogMessage(QString("📋 %1 added to queue (position %2)")
                          .arg(title)
                          .arg(position),
                      "INFO");

        // Refresh auction list
        if (currentUser.isInRoom())
        {
            network->sendListAuctions(currentUser.currentRoomId);
        }
    }
    else if (type == "NOTIF_AUCTION_START")
    {
        // Format: auctionId|title|seller|startPrice|buyNowPrice|minIncrement|duration
        QString title = parts.value(1);
        QString seller = parts.value(2);

        addLogMessage(QString("🎬 Auction started: %1 by %2")
                          .arg(title)
                          .arg(seller),
                      "SUCCESS");

        // Optional: Show popup
        QMessageBox::information(this, "Auction Started",
                                 QString("🎬 Next auction has started!\n\n%1\nSeller: %2")
                                     .arg(title)
                                     .arg(seller));

        // Refresh list
        if (currentUser.isInRoom())
        {
            network->sendListAuctions(currentUser.currentRoomId);
        }
    }
    else if (type == "NOTIF_QUEUE_EMPTY")
    {
        addLogMessage("📭 Queue is empty - No more auctions", "INFO");
    }
}
void MainWindow::onNewBid(int auctionId, double amount, const QString &bidder)
{
    Q_UNUSED(auctionId); // ← Đang bỏ qua auctionId!

    // Tìm tên auction
    QString auctionTitle = "sản phẩm";
    for (const Auction &a : auctions)
    {
        if (a.auctionId == auctionId)
        {
            auctionTitle = a.title;
            break;
        }
    }

    addLogMessage(QString("💰 %1 đặt %2 cho '%3'")
                      .arg(bidder)
                      .arg(Formatters::formatCurrency(amount))
                      .arg(auctionTitle),
                  "BID");

    // Refresh
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onNewAuction(int auctionId, const QString &title)
{
    Q_UNUSED(auctionId);
    addLogMessage(QString("Mới: %1").arg(title), "INFO");
    if (currentUser.isInRoom())
    {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionWarning(int auctionId, int secondsLeft)
{
    Q_UNUSED(auctionId);
    // FIX: Only show if > 0
    if (secondsLeft > 0)
    {
        addLogMessage(QString("Sắp kết thúc: %1s").arg(secondsLeft), "WARNING");
    }
}
void MainWindow::showBidHistoryDialog(const QString &history)
{
    if (history.isEmpty())
    {
        QMessageBox::information(this, "Lịch sử đặt giá",
                                 "Chưa có lượt đặt giá nào cho sản phẩm này");
        return;
    }

    // Parse: bidId;username;amount;timestamp|...
    QStringList bids = history.split('|', Qt::SkipEmptyParts);

    // Create dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Lịch Sử Đặt Giá");
    dialog->setMinimumSize(700, 500);
    dialog->setStyleSheet("QDialog { background: #f5f7fa; }");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Title
    QLabel *title = new QLabel("📊 LỊCH SỬ ĐẶT GIÁ");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #1976d2; "
        "padding: 15px; background: white; border-radius: 8px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Table
    QTableWidget *table = new QTableWidget(bids.size(), 5, dialog);
    table->setHorizontalHeaderLabels({"#", "👤 Người đặt", "💰 Giá đặt", "📈 Tăng", "🕐 Thời gian"});
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #667eea, stop:1 #764ba2); color: white; padding: 10px; "
        "font-weight: bold; border: none; } "
        "QTableWidget::item { padding: 8px; } "
        "QTableWidget::item:selected { background: #e3f2fd; color: #1976d2; }");
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    double prevPrice = 0;
    int row = 0;
    for (const QString &bid : bids)
    {
        QStringList fields = bid.split(';');
        if (fields.size() >= 4)
        {
            QString username = fields[1];
            double amount = fields[2].toDouble();
            qint64 timestamp = fields[3].toLongLong();
            QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);

            // Column 0: #
            QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(row + 1));
            numItem->setTextAlignment(Qt::AlignCenter);
            numItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 0, numItem);

            // Column 1: Username
            QTableWidgetItem *userItem = new QTableWidgetItem(username);
            userItem->setFont(QFont("Arial", 11, QFont::Bold));
            userItem->setForeground(QColor("#1976d2"));
            table->setItem(row, 1, userItem);

            // Column 2: Amount
            QTableWidgetItem *amountItem = new QTableWidgetItem(
                Formatters::formatCurrency(amount));
            amountItem->setFont(QFont("Arial", 12, QFont::Bold));
            amountItem->setForeground(QColor("#4caf50"));
            amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(row, 2, amountItem);

            // Column 3: Increase
            QString increase = "−";
            if (prevPrice > 0)
            {
                double diff = amount - prevPrice;
                increase = QString("+%1").arg(Formatters::formatCurrency(diff));
            }
            QTableWidgetItem *incItem = new QTableWidgetItem(increase);
            incItem->setForeground(QColor("#f57c00"));
            incItem->setFont(QFont("Arial", 10, QFont::Bold));
            incItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(row, 3, incItem);

            // Column 4: Time
            QTableWidgetItem *timeItem = new QTableWidgetItem(
                dt.toString("dd/MM/yyyy\nhh:mm:ss"));
            timeItem->setFont(QFont("Arial", 10));
            timeItem->setForeground(QColor("#666"));
            table->setItem(row, 4, timeItem);

            prevPrice = amount;
            row++;
        }
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    // Stats
    QLabel *stats = new QLabel(QString(
                                   "📊 Tổng cộng: <b>%1 lượt</b> đặt giá")
                                   .arg(bids.size()));
    stats->setStyleSheet(
        "font-size: 14px; padding: 10px; background: white; "
        "border-radius: 8px; color: #666;");
    layout->addWidget(stats);

    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #4caf50; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #45a049; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    dialog->exec();
    delete dialog;
}

void MainWindow::onDisconnected()
{
    addLogMessage("MẤT KẾT NỐI!", "ERROR");

    if (userHasActiveBids())
    {
        QMessageBox::critical(this, "Mất kết nối",
                              "⚠️ BẠN ĐANG CÓ GIÁ ĐẶT!\n\n"
                              "Kết nối bị mất. Giá của bạn vẫn hiệu lực.\n"
                              "Kết nối lại càng sớm càng tốt!");
    }

    // Offer reconnect
    QTimer::singleShot(3000, this, [this]()
                       {
        if (!network->isConnected()) {
            auto reply = QMessageBox::question(this, "Kết nối lại",
                "Kết nối lại?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                // Reconnect...
            }
        } });
}
void MainWindow::showAuctionEndedPopup(const Auction &auction,
                                       const QString &winner,
                                       double finalPrice)
{
    bool userWon = (winner == currentUser.username);
    bool userParticipated = false;

    // Check if user placed any bid
    for (const Auction &a : auctions)
    {
        if (a.auctionId == auction.auctionId)
        {
            // Simplified: assume participated if totalBids > 0 and user in room
            userParticipated = (a.totalBids > 0);
            break;
        }
    }

    // Only show popup if user won or participated
    if (!userWon && !userParticipated)
    {
        // Just log for observers
        addLogMessage(QString("🎉 '%1' kết thúc - %2 thắng với %3")
                          .arg(auction.title)
                          .arg(winner)
                          .arg(Formatters::formatCurrency(finalPrice)),
                      "WIN");
        return;
    }

    // Create detailed popup
    QString icon = userWon ? "🎉" : "😢";
    QString title = userWon ? "CHÚC MỪNG BẠN!" : "KẾT QUẢ ĐẤU GIÁ";
    QString bgColor = userWon ? "#e8f5e9" : "#fff3e0";
    QString titleColor = userWon ? "#4caf50" : "#f57c00";

    QString message = QString(
                          "<div style='background: %1; padding: 20px; border-radius: 10px;'>"
                          "<h2 style='color: %2; margin: 0;'>%3 %4</h2>"
                          "<hr style='border: 1px solid #e0e0e0;'>"
                          "<table style='width: 100%%; font-size: 14px; line-height: 2;'>"
                          "<tr><td style='color: #666;'><b>Sản phẩm:</b></td><td><b>%5</b></td></tr>"
                          "<tr><td style='color: #666;'><b>Giá khởi điểm:</b></td><td>%6</td></tr>"
                          "<tr><td style='color: #666;'><b>Giá cuối:</b></td>"
                          "<td style='color: #4caf50; font-size: 16px;'><b>%7</b></td></tr>"
                          "<tr><td style='color: #666;'><b>Người thắng:</b></td>"
                          "<td style='color: #1976d2;'><b>%8</b></td></tr>"
                          "<tr><td style='color: #666;'><b>Tổng lượt đặt:</b></td><td>%9</td></tr>"
                          "</table>"
                          "<hr style='border: 1px solid #e0e0e0;'>"
                          "<p style='font-size: 14px; color: #666; margin: 10px 0;'>%10</p>"
                          "</div>")
                          .arg(bgColor)
                          .arg(titleColor)
                          .arg(icon)
                          .arg(title)
                          .arg(auction.title)
                          .arg(Formatters::formatCurrency(auction.startPrice))
                          .arg(Formatters::formatCurrency(finalPrice))
                          .arg(winner) // ← USERNAME sẽ hiện ở đây
                          .arg(auction.totalBids)
                          .arg(userWon ? QString("🎉 Chúc mừng <b>%1</b>, bạn đã thắng đấu giá!").arg(currentUser.username) : QString("Người dùng <b>%1</b> đã đặt giá cao hơn bạn.").arg(winner));

    QMessageBox *endBox = new QMessageBox(this);
    endBox->setWindowTitle(title);
    endBox->setText(message);
    endBox->setIcon(userWon ? QMessageBox::Information : QMessageBox::Warning);
    endBox->setStandardButtons(QMessageBox::Ok);
    endBox->exec();
    delete endBox;
}
void MainWindow::onAuctionEnded(int auctionId, const QString &winner, double finalPrice)
{
    qDebug() << "[DEBUG] Auction ended:" << auctionId << "Winner:" << winner;

    // Remove from warned list
    warnedAuctions.remove(auctionId);

    // Find auction for popup
    Auction endedAuction;
    for (const Auction &a : auctions)
    {
        if (a.auctionId == auctionId)
        {
            endedAuction = a;
            break;
        }
    }

    // Show popup if auction found
    if (endedAuction.auctionId > 0)
    {
        showAuctionEndedPopup(endedAuction, winner, finalPrice);
    }

    addLogMessage(QString("🎉 %1 thắng: %2")
                      .arg(winner)
                      .arg(Formatters::formatCurrency(finalPrice)),
                  "WIN");

    // REMOVE from UI list immediately
    for (int i = 0; i < auctionsList->count(); i++)
    {
        QListWidgetItem *item = auctionsList->item(i);
        if (item && item->data(Qt::UserRole).toInt() == auctionId)
        {
            qDebug() << "[UI] Removing ended auction" << auctionId << "from list at index" << i;
            delete auctionsList->takeItem(i);
            break;
        }
    }

    // REMOVE from data array
    for (int i = 0; i < auctions.size(); i++)
    {
        if (auctions[i].auctionId == auctionId)
        {
            qDebug() << "[DATA] Removing auction" << auctionId << "from array at index" << i;
            auctions.removeAt(i);
            break;
        }
    }

    // Refresh to sync with server
    if (currentUser.isInRoom())
    {
        qDebug() << "[REFRESH] Requesting fresh auction list";
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onUserJoinedRoom(const QString &username)
{
    addLogMessage(QString("👋 %1 vào phòng").arg(username), "INFO");

    // Refresh room list để update số người
    network->sendListRooms();
}

void MainWindow::onUserLeftRoom(const QString &username)
{
    addLogMessage(QString("%1 rời").arg(username), "INFO");
    // FIX: Update room list
    network->sendListRooms();
}

void MainWindow::onRoomError(const QString &error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::onAuctionError(const QString &error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::onBidError(const QString &error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::updateCountdowns()
{
    if (currentUser.isInRoom() && !auctions.isEmpty())
    {
        int currentRow = auctionsList->currentRow();

        int displayIndex = 0;
        for (int i = 0; i < auctions.size(); i++)
        {
            if (auctions[i].isActive())
            {
                if (displayIndex < auctionsList->count())
                {
                    QString status = Formatters::formatTime(auctions[i].getTimeLeft());
                    QString text = QString("🔨 %1 | 💰 %2 | ⏱️ %3")
                                       .arg(auctions[i].title)
                                       .arg(Formatters::formatCurrency(auctions[i].currentPrice))
                                       .arg(status);
                    auctionsList->item(displayIndex)->setText(text);
                    displayIndex++;
                }
            }
        }

        if (currentRow >= 0)
        {
            auctionsList->setCurrentRow(currentRow);
        }
    }
}