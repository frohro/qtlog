/****************************************************************************
** Meta object code from reading C++ file 'confDiag.h'
**
** Created: Thu Feb 16 20:58:49 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../confDiag.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'confDiag.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_confDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,
      31,   11,   11,   11, 0x09,
      47,   11,   11,   11, 0x09,
      61,   11,   11,   11, 0x09,
      79,   77,   11,   11, 0x09,
     113,   11,   11,   11, 0x09,
     130,   11,   11,   11, 0x09,
     150,   11,   11,   11, 0x09,
     159,   11,   11,   11, 0x09,
     179,   11,   11,   11, 0x09,
     201,   11,   11,   11, 0x09,
     241,   11,   11,   11, 0x09,
     273,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_confDialog[] = {
    "confDialog\0\0textMoved(QString)\0"
    "buttonClicked()\0makeLogFile()\0"
    "deleteLogFile()\0,\0changeLogCb(QTreeWidgetItem*,int)\0"
    "wCallCb(QString)\0wLocatorCb(QString)\0"
    "goExit()\0comboBoxHomeCb(int)\0"
    "internetParamSaveCb()\0"
    "updateLocationItemCb(QTableWidgetItem*)\0"
    "saveColValCb(QTableWidgetItem*)\0"
    "getHilfeCb()\0"
};

void confDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        confDialog *_t = static_cast<confDialog *>(_o);
        switch (_id) {
        case 0: _t->textMoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->buttonClicked(); break;
        case 2: _t->makeLogFile(); break;
        case 3: _t->deleteLogFile(); break;
        case 4: _t->changeLogCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->wCallCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->wLocatorCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->goExit(); break;
        case 8: _t->comboBoxHomeCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->internetParamSaveCb(); break;
        case 10: _t->updateLocationItemCb((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->saveColValCb((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 12: _t->getHilfeCb(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData confDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject confDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_confDialog,
      qt_meta_data_confDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &confDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *confDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *confDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_confDialog))
        return static_cast<void*>(const_cast< confDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int confDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
