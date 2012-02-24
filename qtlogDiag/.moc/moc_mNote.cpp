/****************************************************************************
** Meta object code from reading C++ file 'mNote.h'
**
** Created: Fri Feb 24 14:20:58 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mNote.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mNote.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mNote[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x09,
      41,    6,    6,    6, 0x09,
      58,    6,    6,    6, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_mNote[] = {
    "mNote\0\0updateNoteItem(QTableWidgetItem*)\0"
    "deleteNoteItem()\0goExit()\0"
};

const QMetaObject mNote::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_mNote,
      qt_meta_data_mNote, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mNote::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mNote::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mNote::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mNote))
        return static_cast<void*>(const_cast< mNote*>(this));
    return QDialog::qt_metacast(_clname);
}

int mNote::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateNoteItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: deleteNoteItem(); break;
        case 2: goExit(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
