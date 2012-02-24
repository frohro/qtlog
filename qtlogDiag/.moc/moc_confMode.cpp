/****************************************************************************
** Meta object code from reading C++ file 'confMode.h'
**
** Created: Fri Feb 24 14:20:56 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../confMode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'confMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_confMode[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x09,
      21,   19,    9,    9, 0x09,
      54,   19,    9,    9, 0x09,
      87,   19,    9,    9, 0x09,
     119,   19,    9,    9, 0x09,
     152,   19,    9,    9, 0x09,
     184,   19,    9,    9, 0x09,
     220,   19,    9,    9, 0x09,
     252,    9,    9,    9, 0x09,
     268,    9,    9,    9, 0x09,
     285,    9,    9,    9, 0x09,
     302,    9,    9,    9, 0x09,
     317,    9,    9,    9, 0x09,
     334,    9,    9,    9, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_confMode[] = {
    "confMode\0\0goExit()\0,\0"
    "bandListCb(QTreeWidgetItem*,int)\0"
    "modeListCb(QTreeWidgetItem*,int)\0"
    "pwrListCb(QTreeWidgetItem*,int)\0"
    "lineListCb(QTreeWidgetItem*,int)\0"
    "awdListCb(QTreeWidgetItem*,int)\0"
    "customsListCb(QTreeWidgetItem*,int)\0"
    "rigListCb(QTreeWidgetItem*,int)\0"
    "RigSaveButton()\0RigClearButton()\0"
    "SaveConfValues()\0NewAwdValues()\0"
    "getHilfeModeCb()\0currentIndexchangedCb(int)\0"
};

const QMetaObject confMode::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_confMode,
      qt_meta_data_confMode, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &confMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *confMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *confMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_confMode))
        return static_cast<void*>(const_cast< confMode*>(this));
    return QDialog::qt_metacast(_clname);
}

int confMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goExit(); break;
        case 1: bandListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: modeListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: pwrListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: lineListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: awdListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: customsListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: rigListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: RigSaveButton(); break;
        case 9: RigClearButton(); break;
        case 10: SaveConfValues(); break;
        case 11: NewAwdValues(); break;
        case 12: getHilfeModeCb(); break;
        case 13: currentIndexchangedCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
