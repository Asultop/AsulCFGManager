/****************************************************************************
** Meta object code from reading C++ file 'T_Home.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Headers/T_Home.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'T_Home.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASST_HomeENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASST_HomeENDCLASS = QtMocHelpers::stringData(
    "T_Home",
    "",
    "QWidget*",
    "parent"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASST_HomeENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       2,   18, // constructors
       0,       // flags
       0,       // signalCount

 // constructors: parameters
    0x80000000 | 1, 0x80000000 | 2,    3,
    0x80000000 | 1,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    1,   14,    1, 0x0e,    1 /* Public */,
       0,    0,   17,    1, 0x2e,    2 /* Public | MethodCloned */,

       0        // eod
};

Q_CONSTINIT const QMetaObject T_Home::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseScrollPage::staticMetaObject>(),
    qt_meta_stringdata_CLASST_HomeENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASST_HomeENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASST_HomeENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<T_Home, std::true_type>,
        // constructor 'T_Home'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>
    >,
    nullptr
} };

void T_Home::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { T_Home *_r = new T_Home((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { T_Home *_r = new T_Home();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::ConstructInPlace) {
        switch (_id) {
        case 0: { new (_a[0]) T_Home((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); } break;
        case 1: { new (_a[0]) T_Home(); } break;
        default: break;
        }
    }
    (void)_o;
}

const QMetaObject *T_Home::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *T_Home::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASST_HomeENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return BaseScrollPage::qt_metacast(_clname);
}

int T_Home::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseScrollPage::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
