/****************************************************************************
** Meta object code from reading C++ file 'W_DragFrame.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Headers/W_DragFrame.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'W_DragFrame.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSW_DragFrameENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSW_DragFrameENDCLASS = QtMocHelpers::stringData(
    "W_DragFrame",
    "fileDropped",
    "",
    "filename",
    "QWidget*",
    "parent"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSW_DragFrameENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       2,   27, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // constructors: parameters
    0x80000000 | 2, 0x80000000 | 4,    5,
    0x80000000 | 2,

 // constructors: name, argc, parameters, tag, flags, initial metatype offsets
       0,    1,   23,    2, 0x0e,    3 /* Public */,
       0,    0,   26,    2, 0x2e,    4 /* Public | MethodCloned */,

       0        // eod
};

Q_CONSTINIT const QMetaObject W_DragFrame::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSW_DragFrameENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSW_DragFrameENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSW_DragFrameENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<W_DragFrame, std::true_type>,
        // method 'fileDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // constructor 'W_DragFrame'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>
    >,
    nullptr
} };

void W_DragFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { W_DragFrame *_r = new W_DragFrame((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { W_DragFrame *_r = new W_DragFrame();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    } else if (_c == QMetaObject::ConstructInPlace) {
        switch (_id) {
        case 0: { new (_a[0]) W_DragFrame((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); } break;
        case 1: { new (_a[0]) W_DragFrame(); } break;
        default: break;
        }
    } else if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<W_DragFrame *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->fileDropped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (W_DragFrame::*)(const QString & );
            if (_t _q_method = &W_DragFrame::fileDropped; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *W_DragFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *W_DragFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSW_DragFrameENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int W_DragFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void W_DragFrame::fileDropped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
