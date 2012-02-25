/****************************************************************************
** Meta object code from reading C++ file 'confDiag.h'
**
** Created: Sat Feb 25 15:10:11 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../confDiag.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'confDiag.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_confDialog[] = {

 // content:
       5,       // revision
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

const QMetaObject confDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_confDialog,
      qt_meta_data_confDialog, 0 }
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
        switch (_id) {
        case 0: textMoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: buttonClicked(); break;
        case 2: makeLogFile(); break;
        case 3: deleteLogFile(); break;
        case 4: changeLogCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: wCallCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: wLocatorCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: goExit(); break;
        case 8: comboBoxHomeCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: internetParamSaveCb(); break;
        case 10: updateLocationItemCb((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 11: saveColValCb((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 12: getHilfeCb(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
