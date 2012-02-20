/****************************************************************************
** Meta object code from reading C++ file 'rigctl.h'
**
** Created: Thu Feb 16 20:58:59 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../rigctl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rigctl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_rigctl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x09,
      17,    7,    7,    7, 0x09,
      27,    7,    7,    7, 0x09,
      41,    7,    7,    7, 0x09,
      55,    7,    7,    7, 0x09,
      83,    7,    7,    7, 0x09,
     104,  102,    7,    7, 0x09,
     140,    7,    7,    7, 0x09,
     150,    7,    7,    7, 0x09,
     159,    7,    7,    7, 0x09,
     170,    7,    7,    7, 0x09,
     186,    7,    7,    7, 0x09,
     204,    7,    7,    7, 0x09,
     217,    7,    7,    7, 0x09,
     231,    7,    7,    7, 0x09,
     258,  246,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_rigctl[] = {
    "rigctl\0\0goExit()\0goHilfe()\0showRigList()\0"
    "showRigPage()\0intefaceBoxChanged(QString)\0"
    "hamlibServerInit()\0,\0"
    "itemClickedCb(QTreeWidgetItem*,int)\0"
    "goStart()\0goStop()\0hlibInfo()\0"
    "hlibReadError()\0hlibNoInstalled()\0"
    "requestRig()\0sentCommand()\0readRigReply()\0"
    "socketError\0displayError(QAbstractSocket::SocketError)\0"
};

void rigctl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        rigctl *_t = static_cast<rigctl *>(_o);
        switch (_id) {
        case 0: _t->goExit(); break;
        case 1: _t->goHilfe(); break;
        case 2: _t->showRigList(); break;
        case 3: _t->showRigPage(); break;
        case 4: _t->intefaceBoxChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->hamlibServerInit(); break;
        case 6: _t->itemClickedCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->goStart(); break;
        case 8: _t->goStop(); break;
        case 9: _t->hlibInfo(); break;
        case 10: _t->hlibReadError(); break;
        case 11: _t->hlibNoInstalled(); break;
        case 12: _t->requestRig(); break;
        case 13: _t->sentCommand(); break;
        case 14: _t->readRigReply(); break;
        case 15: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData rigctl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject rigctl::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_rigctl,
      qt_meta_data_rigctl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &rigctl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *rigctl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *rigctl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_rigctl))
        return static_cast<void*>(const_cast< rigctl*>(this));
    return QDialog::qt_metacast(_clname);
}

int rigctl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
