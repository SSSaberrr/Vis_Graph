/****************************************************************************
** Meta object code from reading C++ file 'DataItemEditWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../DataItemEditWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataItemEditWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataItemEditWidget_t {
    QByteArrayData data[12];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataItemEditWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataItemEditWidget_t qt_meta_stringdata_DataItemEditWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "DataItemEditWidget"
QT_MOC_LITERAL(1, 19, 20), // "onSliderValueChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 5), // "value"
QT_MOC_LITERAL(4, 47, 15), // "onSliderRelease"
QT_MOC_LITERAL(5, 63, 16), // "onLineEditReturn"
QT_MOC_LITERAL(6, 80, 17), // "onCheckBoxChanged"
QT_MOC_LITERAL(7, 98, 14), // "onColorChanged"
QT_MOC_LITERAL(8, 113, 24), // "onEnumRadioButtonChanged"
QT_MOC_LITERAL(9, 138, 21), // "onEnumComboBoxChanged"
QT_MOC_LITERAL(10, 160, 3), // "idx"
QT_MOC_LITERAL(11, 164, 15) // "onButtonClicked"

    },
    "DataItemEditWidget\0onSliderValueChanged\0"
    "\0value\0onSliderRelease\0onLineEditReturn\0"
    "onCheckBoxChanged\0onColorChanged\0"
    "onEnumRadioButtonChanged\0onEnumComboBoxChanged\0"
    "idx\0onButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataItemEditWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    0,   57,    2, 0x0a /* Public */,
       5,    0,   58,    2, 0x0a /* Public */,
       6,    1,   59,    2, 0x0a /* Public */,
       7,    1,   62,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    1,   66,    2, 0x0a /* Public */,
      11,    0,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,

       0        // eod
};

void DataItemEditWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataItemEditWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onSliderRelease(); break;
        case 2: _t->onLineEditReturn(); break;
        case 3: _t->onCheckBoxChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onColorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->onEnumRadioButtonChanged(); break;
        case 6: _t->onEnumComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onButtonClicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataItemEditWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DataItemEditWidget.data,
    qt_meta_data_DataItemEditWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataItemEditWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataItemEditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataItemEditWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DataItemEditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
