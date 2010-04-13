/****************************************************************************
** Meta object code from reading C++ file 'imageview.h'
**
** Created: Tue Apr 13 11:58:43 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../imageview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imageview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   12, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // properties: name, type, flags
      17,   10, 0x06095103,
      28,   10, 0x06095103,
      38,   10, 0x06095103,
      46,   10, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_ImageView[] = {
    "ImageView\0double\0zoomFactor\0zoomScale\0"
    "zoomMax\0zoomMin\0"
};

const QMetaObject ImageView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_ImageView,
      qt_meta_data_ImageView, 0 }
};

const QMetaObject *ImageView::metaObject() const
{
    return &staticMetaObject;
}

void *ImageView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageView))
        return static_cast<void*>(const_cast< ImageView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int ImageView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = zoomFactor(); break;
        case 1: *reinterpret_cast< double*>(_v) = zoomScale(); break;
        case 2: *reinterpret_cast< double*>(_v) = zoomMax(); break;
        case 3: *reinterpret_cast< double*>(_v) = zoomMin(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setZoomFactor(*reinterpret_cast< double*>(_v)); break;
        case 1: setZoomScale(*reinterpret_cast< double*>(_v)); break;
        case 2: setZoomMax(*reinterpret_cast< double*>(_v)); break;
        case 3: setZoomMin(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
