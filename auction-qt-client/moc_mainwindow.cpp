/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "windows/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[81];
    char stringdata0[1470];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 29), // "on_refreshRoomsButton_clicked"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 27), // "on_createRoomButton_clicked"
QT_MOC_LITERAL(4, 70, 25), // "on_joinRoomButton_clicked"
QT_MOC_LITERAL(5, 96, 26), // "on_leaveRoomButton_clicked"
QT_MOC_LITERAL(6, 123, 30), // "on_roomsList_itemDoubleClicked"
QT_MOC_LITERAL(7, 154, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(8, 171, 4), // "item"
QT_MOC_LITERAL(9, 176, 33), // "on_roomsList_itemSelectionCha..."
QT_MOC_LITERAL(10, 210, 40), // "on_viewParticipatedHistoryBut..."
QT_MOC_LITERAL(11, 251, 34), // "on_viewSellerHistoryButton_cl..."
QT_MOC_LITERAL(12, 286, 32), // "on_refreshAuctionsButton_clicked"
QT_MOC_LITERAL(13, 319, 30), // "on_createAuctionButton_clicked"
QT_MOC_LITERAL(14, 350, 35), // "on_viewAuctionDetailsButton_c..."
QT_MOC_LITERAL(15, 386, 25), // "on_placeBidButton_clicked"
QT_MOC_LITERAL(16, 412, 23), // "onSearchResultsReceived"
QT_MOC_LITERAL(17, 436, 7), // "results"
QT_MOC_LITERAL(18, 444, 23), // "on_buyNowButton_clicked"
QT_MOC_LITERAL(19, 468, 16), // "onAuctionStarted"
QT_MOC_LITERAL(20, 485, 9), // "auctionId"
QT_MOC_LITERAL(21, 495, 26), // "updateAuctionActionButtons"
QT_MOC_LITERAL(22, 522, 30), // "on_deleteAuctionButton_clicked"
QT_MOC_LITERAL(23, 553, 32), // "on_activateAuctionButton_clicked"
QT_MOC_LITERAL(24, 586, 31), // "on_searchAuctionsButton_clicked"
QT_MOC_LITERAL(25, 618, 27), // "on_bidHistoryButton_clicked"
QT_MOC_LITERAL(26, 646, 36), // "on_auctionsList_itemSelection..."
QT_MOC_LITERAL(27, 683, 32), // "on_viewRoomHistoryButton_clicked"
QT_MOC_LITERAL(28, 716, 25), // "on_roomInfoButton_clicked"
QT_MOC_LITERAL(29, 742, 28), // "on_viewHistoryButton_clicked"
QT_MOC_LITERAL(30, 771, 23), // "on_logoutButton_clicked"
QT_MOC_LITERAL(31, 795, 21), // "onRoomHistoryReceived"
QT_MOC_LITERAL(32, 817, 7), // "history"
QT_MOC_LITERAL(33, 825, 23), // "onSellerHistoryReceived"
QT_MOC_LITERAL(34, 849, 18), // "onRoomListReceived"
QT_MOC_LITERAL(35, 868, 11), // "QList<Room>"
QT_MOC_LITERAL(36, 880, 5), // "rooms"
QT_MOC_LITERAL(37, 886, 12), // "onJoinedRoom"
QT_MOC_LITERAL(38, 899, 6), // "roomId"
QT_MOC_LITERAL(39, 906, 8), // "roomName"
QT_MOC_LITERAL(40, 915, 10), // "onLeftRoom"
QT_MOC_LITERAL(41, 926, 13), // "onRoomCreated"
QT_MOC_LITERAL(42, 940, 21), // "onAuctionListReceived"
QT_MOC_LITERAL(43, 962, 14), // "QList<Auction>"
QT_MOC_LITERAL(44, 977, 8), // "auctions"
QT_MOC_LITERAL(45, 986, 16), // "onAuctionCreated"
QT_MOC_LITERAL(46, 1003, 18), // "onAuctionActivated"
QT_MOC_LITERAL(47, 1022, 11), // "onBidPlaced"
QT_MOC_LITERAL(48, 1034, 15), // "onBuyNowSuccess"
QT_MOC_LITERAL(49, 1050, 16), // "onAuctionDeleted"
QT_MOC_LITERAL(50, 1067, 16), // "onAuctionDetails"
QT_MOC_LITERAL(51, 1084, 7), // "Auction"
QT_MOC_LITERAL(52, 1092, 7), // "auction"
QT_MOC_LITERAL(53, 1100, 20), // "onBidHistoryReceived"
QT_MOC_LITERAL(54, 1121, 24), // "onAuctionHistoryReceived"
QT_MOC_LITERAL(55, 1146, 16), // "onBalanceUpdated"
QT_MOC_LITERAL(56, 1163, 10), // "newBalance"
QT_MOC_LITERAL(57, 1174, 14), // "onNotification"
QT_MOC_LITERAL(58, 1189, 4), // "type"
QT_MOC_LITERAL(59, 1194, 7), // "message"
QT_MOC_LITERAL(60, 1202, 8), // "onNewBid"
QT_MOC_LITERAL(61, 1211, 6), // "amount"
QT_MOC_LITERAL(62, 1218, 6), // "bidder"
QT_MOC_LITERAL(63, 1225, 12), // "onNewAuction"
QT_MOC_LITERAL(64, 1238, 5), // "title"
QT_MOC_LITERAL(65, 1244, 16), // "onAuctionWarning"
QT_MOC_LITERAL(66, 1261, 11), // "secondsLeft"
QT_MOC_LITERAL(67, 1273, 14), // "onAuctionEnded"
QT_MOC_LITERAL(68, 1288, 6), // "winner"
QT_MOC_LITERAL(69, 1295, 10), // "finalPrice"
QT_MOC_LITERAL(70, 1306, 16), // "onUserJoinedRoom"
QT_MOC_LITERAL(71, 1323, 8), // "username"
QT_MOC_LITERAL(72, 1332, 14), // "onUserLeftRoom"
QT_MOC_LITERAL(73, 1347, 11), // "onRoomError"
QT_MOC_LITERAL(74, 1359, 5), // "error"
QT_MOC_LITERAL(75, 1365, 14), // "onAuctionError"
QT_MOC_LITERAL(76, 1380, 10), // "onBidError"
QT_MOC_LITERAL(77, 1391, 14), // "onDisconnected"
QT_MOC_LITERAL(78, 1406, 16), // "updateCountdowns"
QT_MOC_LITERAL(79, 1423, 20), // "checkAuctionWarnings"
QT_MOC_LITERAL(80, 1444, 25) // "onAuctionDeletedBroadcast"

    },
    "MainWindow\0on_refreshRoomsButton_clicked\0"
    "\0on_createRoomButton_clicked\0"
    "on_joinRoomButton_clicked\0"
    "on_leaveRoomButton_clicked\0"
    "on_roomsList_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0"
    "on_roomsList_itemSelectionChanged\0"
    "on_viewParticipatedHistoryButton_clicked\0"
    "on_viewSellerHistoryButton_clicked\0"
    "on_refreshAuctionsButton_clicked\0"
    "on_createAuctionButton_clicked\0"
    "on_viewAuctionDetailsButton_clicked\0"
    "on_placeBidButton_clicked\0"
    "onSearchResultsReceived\0results\0"
    "on_buyNowButton_clicked\0onAuctionStarted\0"
    "auctionId\0updateAuctionActionButtons\0"
    "on_deleteAuctionButton_clicked\0"
    "on_activateAuctionButton_clicked\0"
    "on_searchAuctionsButton_clicked\0"
    "on_bidHistoryButton_clicked\0"
    "on_auctionsList_itemSelectionChanged\0"
    "on_viewRoomHistoryButton_clicked\0"
    "on_roomInfoButton_clicked\0"
    "on_viewHistoryButton_clicked\0"
    "on_logoutButton_clicked\0onRoomHistoryReceived\0"
    "history\0onSellerHistoryReceived\0"
    "onRoomListReceived\0QList<Room>\0rooms\0"
    "onJoinedRoom\0roomId\0roomName\0onLeftRoom\0"
    "onRoomCreated\0onAuctionListReceived\0"
    "QList<Auction>\0auctions\0onAuctionCreated\0"
    "onAuctionActivated\0onBidPlaced\0"
    "onBuyNowSuccess\0onAuctionDeleted\0"
    "onAuctionDetails\0Auction\0auction\0"
    "onBidHistoryReceived\0onAuctionHistoryReceived\0"
    "onBalanceUpdated\0newBalance\0onNotification\0"
    "type\0message\0onNewBid\0amount\0bidder\0"
    "onNewAuction\0title\0onAuctionWarning\0"
    "secondsLeft\0onAuctionEnded\0winner\0"
    "finalPrice\0onUserJoinedRoom\0username\0"
    "onUserLeftRoom\0onRoomError\0error\0"
    "onAuctionError\0onBidError\0onDisconnected\0"
    "updateCountdowns\0checkAuctionWarnings\0"
    "onAuctionDeletedBroadcast"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      55,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  289,    2, 0x08 /* Private */,
       3,    0,  290,    2, 0x08 /* Private */,
       4,    0,  291,    2, 0x08 /* Private */,
       5,    0,  292,    2, 0x08 /* Private */,
       6,    1,  293,    2, 0x08 /* Private */,
       9,    0,  296,    2, 0x08 /* Private */,
      10,    0,  297,    2, 0x08 /* Private */,
      11,    0,  298,    2, 0x08 /* Private */,
      12,    0,  299,    2, 0x08 /* Private */,
      13,    0,  300,    2, 0x08 /* Private */,
      14,    0,  301,    2, 0x08 /* Private */,
      15,    0,  302,    2, 0x08 /* Private */,
      16,    1,  303,    2, 0x08 /* Private */,
      18,    0,  306,    2, 0x08 /* Private */,
      19,    1,  307,    2, 0x08 /* Private */,
      21,    0,  310,    2, 0x08 /* Private */,
      22,    0,  311,    2, 0x08 /* Private */,
      23,    0,  312,    2, 0x08 /* Private */,
      24,    0,  313,    2, 0x08 /* Private */,
      25,    0,  314,    2, 0x08 /* Private */,
      26,    0,  315,    2, 0x08 /* Private */,
      27,    0,  316,    2, 0x08 /* Private */,
      28,    0,  317,    2, 0x08 /* Private */,
      29,    0,  318,    2, 0x08 /* Private */,
      30,    0,  319,    2, 0x08 /* Private */,
      31,    1,  320,    2, 0x08 /* Private */,
      33,    1,  323,    2, 0x08 /* Private */,
      34,    1,  326,    2, 0x08 /* Private */,
      37,    2,  329,    2, 0x08 /* Private */,
      40,    0,  334,    2, 0x08 /* Private */,
      41,    1,  335,    2, 0x08 /* Private */,
      42,    1,  338,    2, 0x08 /* Private */,
      45,    1,  341,    2, 0x08 /* Private */,
      46,    0,  344,    2, 0x08 /* Private */,
      47,    0,  345,    2, 0x08 /* Private */,
      48,    0,  346,    2, 0x08 /* Private */,
      49,    0,  347,    2, 0x08 /* Private */,
      50,    1,  348,    2, 0x08 /* Private */,
      53,    1,  351,    2, 0x08 /* Private */,
      54,    1,  354,    2, 0x08 /* Private */,
      55,    1,  357,    2, 0x08 /* Private */,
      57,    2,  360,    2, 0x08 /* Private */,
      60,    3,  365,    2, 0x08 /* Private */,
      63,    2,  372,    2, 0x08 /* Private */,
      65,    2,  377,    2, 0x08 /* Private */,
      67,    3,  382,    2, 0x08 /* Private */,
      70,    1,  389,    2, 0x08 /* Private */,
      72,    1,  392,    2, 0x08 /* Private */,
      73,    1,  395,    2, 0x08 /* Private */,
      75,    1,  398,    2, 0x08 /* Private */,
      76,    1,  401,    2, 0x08 /* Private */,
      77,    0,  404,    2, 0x08 /* Private */,
      78,    0,  405,    2, 0x08 /* Private */,
      79,    0,  406,    2, 0x08 /* Private */,
      80,    2,  407,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, 0x80000000 | 35,   36,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   38,   39,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, 0x80000000 | 43,   44,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 51,   52,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::QString,   32,
    QMetaType::Void, QMetaType::Double,   56,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   58,   59,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::QString,   20,   61,   62,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   20,   64,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   20,   66,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Double,   20,   68,   69,
    QMetaType::Void, QMetaType::QString,   71,
    QMetaType::Void, QMetaType::QString,   71,
    QMetaType::Void, QMetaType::QString,   74,
    QMetaType::Void, QMetaType::QString,   74,
    QMetaType::Void, QMetaType::QString,   74,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   20,   64,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_refreshRoomsButton_clicked(); break;
        case 1: _t->on_createRoomButton_clicked(); break;
        case 2: _t->on_joinRoomButton_clicked(); break;
        case 3: _t->on_leaveRoomButton_clicked(); break;
        case 4: _t->on_roomsList_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->on_roomsList_itemSelectionChanged(); break;
        case 6: _t->on_viewParticipatedHistoryButton_clicked(); break;
        case 7: _t->on_viewSellerHistoryButton_clicked(); break;
        case 8: _t->on_refreshAuctionsButton_clicked(); break;
        case 9: _t->on_createAuctionButton_clicked(); break;
        case 10: _t->on_viewAuctionDetailsButton_clicked(); break;
        case 11: _t->on_placeBidButton_clicked(); break;
        case 12: _t->onSearchResultsReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->on_buyNowButton_clicked(); break;
        case 14: _t->onAuctionStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->updateAuctionActionButtons(); break;
        case 16: _t->on_deleteAuctionButton_clicked(); break;
        case 17: _t->on_activateAuctionButton_clicked(); break;
        case 18: _t->on_searchAuctionsButton_clicked(); break;
        case 19: _t->on_bidHistoryButton_clicked(); break;
        case 20: _t->on_auctionsList_itemSelectionChanged(); break;
        case 21: _t->on_viewRoomHistoryButton_clicked(); break;
        case 22: _t->on_roomInfoButton_clicked(); break;
        case 23: _t->on_viewHistoryButton_clicked(); break;
        case 24: _t->on_logoutButton_clicked(); break;
        case 25: _t->onRoomHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->onSellerHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->onRoomListReceived((*reinterpret_cast< const QList<Room>(*)>(_a[1]))); break;
        case 28: _t->onJoinedRoom((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 29: _t->onLeftRoom(); break;
        case 30: _t->onRoomCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->onAuctionListReceived((*reinterpret_cast< const QList<Auction>(*)>(_a[1]))); break;
        case 32: _t->onAuctionCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->onAuctionActivated(); break;
        case 34: _t->onBidPlaced(); break;
        case 35: _t->onBuyNowSuccess(); break;
        case 36: _t->onAuctionDeleted(); break;
        case 37: _t->onAuctionDetails((*reinterpret_cast< const Auction(*)>(_a[1]))); break;
        case 38: _t->onBidHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->onAuctionHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 40: _t->onBalanceUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 41: _t->onNotification((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 42: _t->onNewBid((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 43: _t->onNewAuction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 44: _t->onAuctionWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 45: _t->onAuctionEnded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 46: _t->onUserJoinedRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 47: _t->onUserLeftRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 48: _t->onRoomError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 49: _t->onAuctionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 50: _t->onBidError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 51: _t->onDisconnected(); break;
        case 52: _t->updateCountdowns(); break;
        case 53: _t->checkAuctionWarnings(); break;
        case 54: _t->onAuctionDeletedBroadcast((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 55;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
