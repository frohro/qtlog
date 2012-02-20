/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created: Mon Jan 3 11:51:39 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../setup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_setup[] = {

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
       7,    6,    6,    6, 0x09,
      16,    6,    6,    6, 0x09,
      26,    6,    6,    6, 0x09,
      44,    6,    6,    6, 0x09,
      63,    6,    6,    6, 0x09,
      85,    6,    6,    6, 0x09,
      99,    6,    6,    6, 0x09,
     114,    6,    6,    6, 0x09,
     129,    6,    6,    6, 0x09,
     148,    6,    6,    6, 0x09,
     162,    6,    6,    6, 0x09,
     179,    6,    6,    6, 0x09,
     192,    6,    6,    6, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_setup[] = {
    "setup\0\0goExit()\0hilfeAc()\0checkBoxParamCb()\0"
    "checkBoxUpdateCb()\0checkBoxReInstallCb()\0"
    "updateParam()\0installQtlog()\0"
    "installOldCb()\0closeParamUpdate()\0"
    "closeUpdate()\0closeReInstall()\0"
    "updatework()\0loadTable(QString)\0"
};

const QMetaObject setup::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_setup,
      qt_meta_data_setup, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &setup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *setup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *setup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_setup))
        return static_cast<void*>(const_cast< setup*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int setup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goExit(); break;
        case 1: hilfeAc(); break;
        case 2: checkBoxParamCb(); break;
        case 3: checkBoxUpdateCb(); break;
        case 4: checkBoxReInstallCb(); break;
        case 5: updateParam(); break;
        case 6: installQtlog(); break;
        case 7: installOldCb(); break;
        case 8: closeParamUpdate(); break;
        case 9: closeUpdate(); break;
        case 10: closeReInstall(); break;
        case 11: updatework(); break;
        case 12: loadTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
