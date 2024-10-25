/****************************************************************************
** Meta object code from reading C++ file 'myvideocapture.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/myvideocapture.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myvideocapture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyVideoCapture_t {
    QByteArrayData data[10];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyVideoCapture_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyVideoCapture_t qt_meta_stringdata_MyVideoCapture = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MyVideoCapture"
QT_MOC_LITERAL(1, 15, 16), // "newPixmapCapture"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "displayHint"
QT_MOC_LITERAL(4, 45, 4), // "EXIT"
QT_MOC_LITERAL(5, 50, 12), // "NotInRequest"
QT_MOC_LITERAL(6, 63, 10), // "OnlineMode"
QT_MOC_LITERAL(7, 74, 13), // "enableButtons"
QT_MOC_LITERAL(8, 88, 9), // "Streaming"
QT_MOC_LITERAL(9, 98, 21) // "showDoorBellFunctions"

    },
    "MyVideoCapture\0newPixmapCapture\0\0"
    "displayHint\0EXIT\0NotInRequest\0OnlineMode\0"
    "enableButtons\0Streaming\0showDoorBellFunctions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyVideoCapture[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,
       6,    0,   58,    2, 0x06 /* Public */,
       7,    0,   59,    2, 0x06 /* Public */,
       8,    0,   60,    2, 0x06 /* Public */,
       9,    0,   61,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyVideoCapture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyVideoCapture *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newPixmapCapture(); break;
        case 1: _t->displayHint(); break;
        case 2: _t->EXIT(); break;
        case 3: _t->NotInRequest(); break;
        case 4: _t->OnlineMode(); break;
        case 5: _t->enableButtons(); break;
        case 6: _t->Streaming(); break;
        case 7: _t->showDoorBellFunctions(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::newPixmapCapture)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::displayHint)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::EXIT)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::NotInRequest)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::OnlineMode)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::enableButtons)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::Streaming)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MyVideoCapture::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyVideoCapture::showDoorBellFunctions)) {
                *result = 7;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MyVideoCapture::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_MyVideoCapture.data,
    qt_meta_data_MyVideoCapture,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyVideoCapture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyVideoCapture::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyVideoCapture.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MyVideoCapture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MyVideoCapture::newPixmapCapture()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MyVideoCapture::displayHint()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MyVideoCapture::EXIT()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MyVideoCapture::NotInRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MyVideoCapture::OnlineMode()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MyVideoCapture::enableButtons()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MyVideoCapture::Streaming()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MyVideoCapture::showDoorBellFunctions()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
