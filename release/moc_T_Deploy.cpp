/****************************************************************************
** Meta object code from reading C++ file 'T_Deploy.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Headers/T_Deploy.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'T_Deploy.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASST_DeployENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASST_DeployENDCLASS = QtMocHelpers::stringData(
    "T_Deploy",
    "targetToFormatHelp",
    "",
    "generateError",
    "content",
    "changeHideAShowState",
    "show",
    "QWidget*",
    "parent",
    "QTemporaryDir*",
    "tempDir"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASST_DeployENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       3,   48, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,
       3,    1,   33,    2, 0x06,    2 /* Public */,
       5,    1,   36,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Bool,    6,

 // constructors: parameters
    0x80000000 | 2, 0x80000000 | 7, 0x80000000 | 9,    8,   10,
    0x80000000 | 2, 0x80000000 | 7,    8,
    0x80000000 | 2,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    2,   39,    2, 0x0e,    6 /* Public */,
       0,    1,   44,    2, 0x2e,    8 /* Public | MethodCloned */,
       0,    0,   47,    2, 0x2e,    9 /* Public | MethodCloned */,

       0        // eod
};

Q_CONSTINIT const QMetaObject T_Deploy::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseScrollPage::staticMetaObject>(),
    qt_meta_stringdata_CLASST_DeployENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASST_DeployENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASST_DeployENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<T_Deploy, std::true_type>,
        // method 'targetToFormatHelp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'generateError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'changeHideAShowState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // constructor 'T_Deploy'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTemporaryDir *, std::false_type>,
        // constructor 'T_Deploy'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>
    >,
    nullptr
} };

void T_Deploy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { T_Deploy *_r = new T_Deploy((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QTemporaryDir*>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { T_Deploy *_r = new T_Deploy((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 2: { T_Deploy *_r = new T_Deploy();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::ConstructInPlace) {
        switch (_id) {
        case 0: { new (_a[0]) T_Deploy((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QTemporaryDir*>>(_a[2]))); } break;
        case 1: { new (_a[0]) T_Deploy((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); } break;
        case 2: { new (_a[0]) T_Deploy(); } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<T_Deploy *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->targetToFormatHelp(); break;
        case 1: _t->generateError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->changeHideAShowState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (T_Deploy::*)();
            if (_t _q_method = &T_Deploy::targetToFormatHelp; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (T_Deploy::*)(QString );
            if (_t _q_method = &T_Deploy::generateError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (T_Deploy::*)(bool );
            if (_t _q_method = &T_Deploy::changeHideAShowState; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *T_Deploy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *T_Deploy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASST_DeployENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return BaseScrollPage::qt_metacast(_clname);
}

int T_Deploy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseScrollPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void T_Deploy::targetToFormatHelp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void T_Deploy::generateError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void T_Deploy::changeHideAShowState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
