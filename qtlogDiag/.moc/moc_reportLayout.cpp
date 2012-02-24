/****************************************************************************
** Meta object code from reading C++ file 'reportLayout.h'
**
** Created: Fri Feb 24 14:20:54 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../reportLayout.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reportLayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_reportLayout[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x09,
      25,   23,   13,   13, 0x09,
      61,   23,   13,   13, 0x09,
     101,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_reportLayout[] = {
    "reportLayout\0\0goExit()\0,\0"
    "dbListClicked(QTreeWidgetItem*,int)\0"
    "layoutListClicked(QTreeWidgetItem*,int)\0"
    "getReportListItemsCb(QString)\0"
};

const QMetaObject reportLayout::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_reportLayout,
      qt_meta_data_reportLayout, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &reportLayout::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *reportLayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *reportLayout::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_reportLayout))
        return static_cast<void*>(const_cast< reportLayout*>(this));
    return QDialog::qt_metacast(_clname);
}

int reportLayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goExit(); break;
        case 1: dbListClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: layoutListClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: getReportListItemsCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
