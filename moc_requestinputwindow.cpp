/****************************************************************************
** Meta object code from reading C++ file 'requestinputwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/requestinputwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'requestinputwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RequestInputWindow_t {
    QByteArrayData data[12];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RequestInputWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RequestInputWindow_t qt_meta_stringdata_RequestInputWindow = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RequestInputWindow"
QT_MOC_LITERAL(1, 19, 13), // "buttonPressed"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 8), // "_purpose"
QT_MOC_LITERAL(4, 43, 4), // "Done"
QT_MOC_LITERAL(5, 48, 19), // "on_pB_Visit_clicked"
QT_MOC_LITERAL(6, 68, 24), // "on_pB_DropParcel_clicked"
QT_MOC_LITERAL(7, 93, 26), // "on_pB_FoodDelivery_clicked"
QT_MOC_LITERAL(8, 120, 22), // "on_pB_Services_clicked"
QT_MOC_LITERAL(9, 143, 20), // "on_pB_PickUp_clicked"
QT_MOC_LITERAL(10, 164, 25), // "on_pB_OtherReason_clicked"
QT_MOC_LITERAL(11, 190, 18) // "on_pB_Back_clicked"

    },
    "RequestInputWindow\0buttonPressed\0\0"
    "_purpose\0Done\0on_pB_Visit_clicked\0"
    "on_pB_DropParcel_clicked\0"
    "on_pB_FoodDelivery_clicked\0"
    "on_pB_Services_clicked\0on_pB_PickUp_clicked\0"
    "on_pB_OtherReason_clicked\0on_pB_Back_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RequestInputWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RequestInputWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RequestInputWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->buttonPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Done(); break;
        case 2: _t->on_pB_Visit_clicked(); break;
        case 3: _t->on_pB_DropParcel_clicked(); break;
        case 4: _t->on_pB_FoodDelivery_clicked(); break;
        case 5: _t->on_pB_Services_clicked(); break;
        case 6: _t->on_pB_PickUp_clicked(); break;
        case 7: _t->on_pB_OtherReason_clicked(); break;
        case 8: _t->on_pB_Back_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RequestInputWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RequestInputWindow::buttonPressed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RequestInputWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RequestInputWindow::Done)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RequestInputWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_RequestInputWindow.data,
    qt_meta_data_RequestInputWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RequestInputWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RequestInputWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RequestInputWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RequestInputWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void RequestInputWindow::buttonPressed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RequestInputWindow::Done()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
