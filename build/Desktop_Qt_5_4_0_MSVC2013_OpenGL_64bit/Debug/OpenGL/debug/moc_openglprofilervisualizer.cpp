/****************************************************************************
** Meta object code from reading C++ file 'openglprofilervisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../OpenGL/openglprofilervisualizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openglprofilervisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OpenGLProfilerVisualizer_t {
    QByteArrayData data[5];
    char stringdata[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenGLProfilerVisualizer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenGLProfilerVisualizer_t qt_meta_stringdata_OpenGLProfilerVisualizer = {
    {
QT_MOC_LITERAL(0, 0, 24), // "OpenGLProfilerVisualizer"
QT_MOC_LITERAL(1, 25, 21), // "frameResultsAvailable"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 18), // "OpenGLFrameResults"
QT_MOC_LITERAL(4, 67, 7) // "results"

    },
    "OpenGLProfilerVisualizer\0frameResultsAvailable\0"
    "\0OpenGLFrameResults\0results"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGLProfilerVisualizer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void OpenGLProfilerVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OpenGLProfilerVisualizer *_t = static_cast<OpenGLProfilerVisualizer *>(_o);
        switch (_id) {
        case 0: _t->frameResultsAvailable((*reinterpret_cast< const OpenGLFrameResults(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject OpenGLProfilerVisualizer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OpenGLProfilerVisualizer.data,
      qt_meta_data_OpenGLProfilerVisualizer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OpenGLProfilerVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLProfilerVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLProfilerVisualizer.stringdata))
        return static_cast<void*>(const_cast< OpenGLProfilerVisualizer*>(this));
    return QObject::qt_metacast(_clname);
}

int OpenGLProfilerVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
