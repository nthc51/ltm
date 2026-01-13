/****************************************************************************
** Meta object code from reading C++ file 'networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "core/networkmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkManager_t {
    QByteArrayData data[71];
    char stringdata0[860];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkManager_t qt_meta_stringdata_NetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetworkManager"
QT_MOC_LITERAL(1, 15, 9), // "connected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "disconnected"
QT_MOC_LITERAL(4, 39, 15), // "connectionError"
QT_MOC_LITERAL(5, 55, 5), // "error"
QT_MOC_LITERAL(6, 61, 21), // "searchResultsReceived"
QT_MOC_LITERAL(7, 83, 7), // "results"
QT_MOC_LITERAL(8, 91, 14), // "auctionStarted"
QT_MOC_LITERAL(9, 106, 9), // "auctionId"
QT_MOC_LITERAL(10, 116, 12), // "loginSuccess"
QT_MOC_LITERAL(11, 129, 4), // "User"
QT_MOC_LITERAL(12, 134, 4), // "user"
QT_MOC_LITERAL(13, 139, 11), // "loginFailed"
QT_MOC_LITERAL(14, 151, 6), // "reason"
QT_MOC_LITERAL(15, 158, 15), // "registerSuccess"
QT_MOC_LITERAL(16, 174, 14), // "registerFailed"
QT_MOC_LITERAL(17, 189, 13), // "logoutSuccess"
QT_MOC_LITERAL(18, 203, 23), // "auctionDeletedBroadcast"
QT_MOC_LITERAL(19, 227, 5), // "title"
QT_MOC_LITERAL(20, 233, 21), // "sellerHistoryReceived"
QT_MOC_LITERAL(21, 255, 7), // "history"
QT_MOC_LITERAL(22, 263, 19), // "roomHistoryReceived"
QT_MOC_LITERAL(23, 283, 11), // "roomCreated"
QT_MOC_LITERAL(24, 295, 6), // "roomId"
QT_MOC_LITERAL(25, 302, 16), // "roomListReceived"
QT_MOC_LITERAL(26, 319, 11), // "QList<Room>"
QT_MOC_LITERAL(27, 331, 5), // "rooms"
QT_MOC_LITERAL(28, 337, 10), // "joinedRoom"
QT_MOC_LITERAL(29, 348, 8), // "roomName"
QT_MOC_LITERAL(30, 357, 8), // "leftRoom"
QT_MOC_LITERAL(31, 366, 9), // "roomError"
QT_MOC_LITERAL(32, 376, 14), // "auctionCreated"
QT_MOC_LITERAL(33, 391, 19), // "auctionListReceived"
QT_MOC_LITERAL(34, 411, 14), // "QList<Auction>"
QT_MOC_LITERAL(35, 426, 8), // "auctions"
QT_MOC_LITERAL(36, 435, 14), // "auctionDetails"
QT_MOC_LITERAL(37, 450, 7), // "Auction"
QT_MOC_LITERAL(38, 458, 7), // "auction"
QT_MOC_LITERAL(39, 466, 14), // "auctionDeleted"
QT_MOC_LITERAL(40, 481, 16), // "auctionActivated"
QT_MOC_LITERAL(41, 498, 12), // "auctionError"
QT_MOC_LITERAL(42, 511, 9), // "bidPlaced"
QT_MOC_LITERAL(43, 521, 8), // "bidError"
QT_MOC_LITERAL(44, 530, 13), // "buyNowSuccess"
QT_MOC_LITERAL(45, 544, 18), // "bidHistoryReceived"
QT_MOC_LITERAL(46, 563, 14), // "balanceUpdated"
QT_MOC_LITERAL(47, 578, 10), // "newBalance"
QT_MOC_LITERAL(48, 589, 22), // "auctionHistoryReceived"
QT_MOC_LITERAL(49, 612, 12), // "notification"
QT_MOC_LITERAL(50, 625, 4), // "type"
QT_MOC_LITERAL(51, 630, 7), // "message"
QT_MOC_LITERAL(52, 638, 14), // "userJoinedRoom"
QT_MOC_LITERAL(53, 653, 8), // "username"
QT_MOC_LITERAL(54, 662, 12), // "userLeftRoom"
QT_MOC_LITERAL(55, 675, 10), // "newAuction"
QT_MOC_LITERAL(56, 686, 6), // "newBid"
QT_MOC_LITERAL(57, 693, 6), // "amount"
QT_MOC_LITERAL(58, 700, 6), // "bidder"
QT_MOC_LITERAL(59, 707, 14), // "auctionWarning"
QT_MOC_LITERAL(60, 722, 11), // "secondsLeft"
QT_MOC_LITERAL(61, 734, 12), // "auctionEnded"
QT_MOC_LITERAL(62, 747, 6), // "winner"
QT_MOC_LITERAL(63, 754, 10), // "finalPrice"
QT_MOC_LITERAL(64, 765, 6), // "kicked"
QT_MOC_LITERAL(65, 772, 11), // "onConnected"
QT_MOC_LITERAL(66, 784, 14), // "onDisconnected"
QT_MOC_LITERAL(67, 799, 11), // "onReadyRead"
QT_MOC_LITERAL(68, 811, 7), // "onError"
QT_MOC_LITERAL(69, 819, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(70, 848, 11) // "socketError"

    },
    "NetworkManager\0connected\0\0disconnected\0"
    "connectionError\0error\0searchResultsReceived\0"
    "results\0auctionStarted\0auctionId\0"
    "loginSuccess\0User\0user\0loginFailed\0"
    "reason\0registerSuccess\0registerFailed\0"
    "logoutSuccess\0auctionDeletedBroadcast\0"
    "title\0sellerHistoryReceived\0history\0"
    "roomHistoryReceived\0roomCreated\0roomId\0"
    "roomListReceived\0QList<Room>\0rooms\0"
    "joinedRoom\0roomName\0leftRoom\0roomError\0"
    "auctionCreated\0auctionListReceived\0"
    "QList<Auction>\0auctions\0auctionDetails\0"
    "Auction\0auction\0auctionDeleted\0"
    "auctionActivated\0auctionError\0bidPlaced\0"
    "bidError\0buyNowSuccess\0bidHistoryReceived\0"
    "balanceUpdated\0newBalance\0"
    "auctionHistoryReceived\0notification\0"
    "type\0message\0userJoinedRoom\0username\0"
    "userLeftRoom\0newAuction\0newBid\0amount\0"
    "bidder\0auctionWarning\0secondsLeft\0"
    "auctionEnded\0winner\0finalPrice\0kicked\0"
    "onConnected\0onDisconnected\0onReadyRead\0"
    "onError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      38,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  224,    2, 0x06 /* Public */,
       3,    0,  225,    2, 0x06 /* Public */,
       4,    1,  226,    2, 0x06 /* Public */,
       6,    1,  229,    2, 0x06 /* Public */,
       8,    1,  232,    2, 0x06 /* Public */,
      10,    1,  235,    2, 0x06 /* Public */,
      13,    1,  238,    2, 0x06 /* Public */,
      15,    0,  241,    2, 0x06 /* Public */,
      16,    1,  242,    2, 0x06 /* Public */,
      17,    0,  245,    2, 0x06 /* Public */,
      18,    2,  246,    2, 0x06 /* Public */,
      20,    1,  251,    2, 0x06 /* Public */,
      22,    1,  254,    2, 0x06 /* Public */,
      23,    1,  257,    2, 0x06 /* Public */,
      25,    1,  260,    2, 0x06 /* Public */,
      28,    2,  263,    2, 0x06 /* Public */,
      30,    0,  268,    2, 0x06 /* Public */,
      31,    1,  269,    2, 0x06 /* Public */,
      32,    1,  272,    2, 0x06 /* Public */,
      33,    1,  275,    2, 0x06 /* Public */,
      36,    1,  278,    2, 0x06 /* Public */,
      39,    0,  281,    2, 0x06 /* Public */,
      40,    0,  282,    2, 0x06 /* Public */,
      41,    1,  283,    2, 0x06 /* Public */,
      42,    0,  286,    2, 0x06 /* Public */,
      43,    1,  287,    2, 0x06 /* Public */,
      44,    0,  290,    2, 0x06 /* Public */,
      45,    1,  291,    2, 0x06 /* Public */,
      46,    1,  294,    2, 0x06 /* Public */,
      48,    1,  297,    2, 0x06 /* Public */,
      49,    2,  300,    2, 0x06 /* Public */,
      52,    1,  305,    2, 0x06 /* Public */,
      54,    1,  308,    2, 0x06 /* Public */,
      55,    2,  311,    2, 0x06 /* Public */,
      56,    3,  316,    2, 0x06 /* Public */,
      59,    2,  323,    2, 0x06 /* Public */,
      61,    3,  328,    2, 0x06 /* Public */,
      64,    1,  335,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      65,    0,  338,    2, 0x08 /* Private */,
      66,    0,  339,    2, 0x08 /* Private */,
      67,    0,  340,    2, 0x08 /* Private */,
      68,    1,  341,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   24,   29,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void, 0x80000000 | 37,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::Double,   47,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   50,   51,
    QMetaType::Void, QMetaType::QString,   53,
    QMetaType::Void, QMetaType::QString,   53,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::QString,    9,   57,   58,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   60,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Double,    9,   62,   63,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 69,   70,

       0        // eod
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->searchResultsReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->auctionStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->loginSuccess((*reinterpret_cast< User(*)>(_a[1]))); break;
        case 6: _t->loginFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->registerSuccess(); break;
        case 8: _t->registerFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->logoutSuccess(); break;
        case 10: _t->auctionDeletedBroadcast((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 11: _t->sellerHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->roomHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->roomCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->roomListReceived((*reinterpret_cast< const QList<Room>(*)>(_a[1]))); break;
        case 15: _t->joinedRoom((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 16: _t->leftRoom(); break;
        case 17: _t->roomError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->auctionCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->auctionListReceived((*reinterpret_cast< const QList<Auction>(*)>(_a[1]))); break;
        case 20: _t->auctionDetails((*reinterpret_cast< const Auction(*)>(_a[1]))); break;
        case 21: _t->auctionDeleted(); break;
        case 22: _t->auctionActivated(); break;
        case 23: _t->auctionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->bidPlaced(); break;
        case 25: _t->bidError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 26: _t->buyNowSuccess(); break;
        case 27: _t->bidHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 28: _t->balanceUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 29: _t->auctionHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 30: _t->notification((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 31: _t->userJoinedRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->userLeftRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: _t->newAuction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 34: _t->newBid((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 35: _t->auctionWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 36: _t->auctionEnded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 37: _t->kicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 38: _t->onConnected(); break;
        case 39: _t->onDisconnected(); break;
        case 40: _t->onReadyRead(); break;
        case 41: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 41:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::searchResultsReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionStarted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(User );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::loginSuccess)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::loginFailed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::registerSuccess)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::registerFailed)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::logoutSuccess)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDeletedBroadcast)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::sellerHistoryReceived)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomHistoryReceived)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomCreated)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QList<Room> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomListReceived)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::joinedRoom)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::leftRoom)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomError)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionCreated)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QList<Auction> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionListReceived)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const Auction & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDetails)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDeleted)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionActivated)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionError)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidPlaced)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidError)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::buyNowSuccess)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidHistoryReceived)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::balanceUpdated)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionHistoryReceived)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::notification)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::userJoinedRoom)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::userLeftRoom)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::newAuction)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , double , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::newBid)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionWarning)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionEnded)) {
                *result = 36;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::kicked)) {
                *result = 37;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkManager.data,
    qt_meta_data_NetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkManager::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkManager::searchResultsReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkManager::auctionStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkManager::loginSuccess(User _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkManager::loginFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkManager::registerSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void NetworkManager::registerFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NetworkManager::logoutSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void NetworkManager::auctionDeletedBroadcast(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkManager::sellerHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkManager::roomHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkManager::roomCreated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void NetworkManager::roomListReceived(const QList<Room> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkManager::joinedRoom(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void NetworkManager::leftRoom()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void NetworkManager::roomError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void NetworkManager::auctionCreated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void NetworkManager::auctionListReceived(const QList<Auction> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void NetworkManager::auctionDetails(const Auction & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void NetworkManager::auctionDeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void NetworkManager::auctionActivated()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void NetworkManager::auctionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void NetworkManager::bidPlaced()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void NetworkManager::bidError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void NetworkManager::buyNowSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void NetworkManager::bidHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void NetworkManager::balanceUpdated(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void NetworkManager::auctionHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void NetworkManager::notification(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void NetworkManager::userJoinedRoom(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void NetworkManager::userLeftRoom(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void NetworkManager::newAuction(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void NetworkManager::newBid(int _t1, double _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void NetworkManager::auctionWarning(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void NetworkManager::auctionEnded(int _t1, const QString & _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void NetworkManager::kicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
