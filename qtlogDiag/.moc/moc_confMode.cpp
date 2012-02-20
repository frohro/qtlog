/****************************************************************************
** Meta object code from reading C++ file 'confMode.h'
**
** Created: Thu Feb 16 20:58:54 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../confMode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'confMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_confMode[] = {

 // content:
       6,       // revision
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

void confMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        confMode *_t = static_cast<confMode *>(_o);
        switch (_id) {
        case 0: _t->goExit(); break;
        case 1: _t->bandListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->modeListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->pwrListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->lineListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->awdListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->customsListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->rigListCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->RigSaveButton(); break;
        case 9: _t->RigClearButton(); break;
        case 10: _t->SaveConfValues(); break;
        case 11: _t->NewAwdValues(); break;
        case 12: _t->getHilfeModeCb(); break;
        case 13: _t->currentIndexchangedCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData confMode::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject confMode::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_confMode,
      qt_meta_data_confMode, &staticMetaObjectExtraData }
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
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
