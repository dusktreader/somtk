/****************************************************************************
** Meta object code from reading C++ file 'megaslider.h'
**
** Created: Tue Jul 13 16:44:05 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../megaslider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'megaslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MegaSlider[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       7,   49, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   12,   11,   11, 0x08,
      56,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
      90,   11,   11,   11, 0x08,
     107,   11,   11,   11, 0x08,
     123,   11,   11,   11, 0x08,

 // properties: name, type, flags
     146,  139, 0x06095103,
     154,  139, 0x06095103,
      12,  139, 0x06095103,
     162,  139, 0x06095103,
     172,  167, 0x01095103,
     195,  181, 0x0009510b,
     213,  205, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_MegaSlider[] = {
    "MegaSlider\0\0value\0valueChanged(double)\0"
    "sliderMoved(int)\0sliderReleased()\0"
    "incrementValue()\0decrementValue()\0"
    "maximizeValue()\0minimizeValue()\0double\0"
    "maximum\0minimum\0step\0bool\0tracking\0"
    "Qt::Alignment\0alignment\0QString\0label\0"
};

const QMetaObject MegaSlider::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_MegaSlider,
      qt_meta_data_MegaSlider, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MegaSlider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MegaSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MegaSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MegaSlider))
        return static_cast<void*>(const_cast< MegaSlider*>(this));
    return QFrame::qt_metacast(_clname);
}

int MegaSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: sliderReleased(); break;
        case 3: incrementValue(); break;
        case 4: decrementValue(); break;
        case 5: maximizeValue(); break;
        case 6: minimizeValue(); break;
        default: ;
        }
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = maximum(); break;
        case 1: *reinterpret_cast< double*>(_v) = minimum(); break;
        case 2: *reinterpret_cast< double*>(_v) = value(); break;
        case 3: *reinterpret_cast< double*>(_v) = step(); break;
        case 4: *reinterpret_cast< bool*>(_v) = tracking(); break;
        case 5: *reinterpret_cast< Qt::Alignment*>(_v) = alignment(); break;
        case 6: *reinterpret_cast< QString*>(_v) = label(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMaximum(*reinterpret_cast< double*>(_v)); break;
        case 1: setMinimum(*reinterpret_cast< double*>(_v)); break;
        case 2: setValue(*reinterpret_cast< double*>(_v)); break;
        case 3: setStep(*reinterpret_cast< double*>(_v)); break;
        case 4: setTracking(*reinterpret_cast< bool*>(_v)); break;
        case 5: setAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 6: setLabel(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MegaSlider::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
