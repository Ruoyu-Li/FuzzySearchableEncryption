/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata[421];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 27),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 18),
QT_MOC_LITERAL(4, 59, 5),
QT_MOC_LITERAL(5, 65, 29),
QT_MOC_LITERAL(6, 95, 27),
QT_MOC_LITERAL(7, 123, 26),
QT_MOC_LITERAL(8, 150, 27),
QT_MOC_LITERAL(9, 178, 28),
QT_MOC_LITERAL(10, 207, 27),
QT_MOC_LITERAL(11, 235, 26),
QT_MOC_LITERAL(12, 262, 21),
QT_MOC_LITERAL(13, 284, 25),
QT_MOC_LITERAL(14, 310, 30),
QT_MOC_LITERAL(15, 341, 32),
QT_MOC_LITERAL(16, 374, 29),
QT_MOC_LITERAL(17, 404, 16)
    },
    "MainWindow\0on_pushButton_creat_clicked\0"
    "\0show_tabWidgetInfo\0index\0"
    "on_pushButton_acquire_clicked\0"
    "on_pushButton_check_clicked\0"
    "on_pushButton_load_clicked\0"
    "on_pushButton_setup_clicked\0"
    "on_pushButton_search_clicked\0"
    "on_pushButton_clear_clicked\0"
    "on_tabWidget_tabBarClicked\0"
    "on_pushButton_clicked\0on_pushButton_add_clicked\0"
    "on_pushButton_deleteid_clicked\0"
    "on_pushButton_delkeyword_clicked\0"
    "on_pushButton_addfile_clicked\0"
    "on_renew_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      13,    0,  103,    2, 0x08 /* Private */,
      14,    0,  104,    2, 0x08 /* Private */,
      15,    0,  105,    2, 0x08 /* Private */,
      16,    0,  106,    2, 0x08 /* Private */,
      17,    0,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_creat_clicked(); break;
        case 1: _t->show_tabWidgetInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_pushButton_acquire_clicked(); break;
        case 3: _t->on_pushButton_check_clicked(); break;
        case 4: _t->on_pushButton_load_clicked(); break;
        case 5: _t->on_pushButton_setup_clicked(); break;
        case 6: _t->on_pushButton_search_clicked(); break;
        case 7: _t->on_pushButton_clear_clicked(); break;
        case 8: _t->on_tabWidget_tabBarClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_pushButton_clicked(); break;
        case 10: _t->on_pushButton_add_clicked(); break;
        case 11: _t->on_pushButton_deleteid_clicked(); break;
        case 12: _t->on_pushButton_delkeyword_clicked(); break;
        case 13: _t->on_pushButton_addfile_clicked(); break;
        case 14: _t->on_renew_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE