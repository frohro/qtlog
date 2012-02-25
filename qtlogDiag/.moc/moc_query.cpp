/****************************************************************************
** Meta object code from reading C++ file 'query.h'
**
** Created: Sat Feb 25 15:10:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../query.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'query.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_query[] = {

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
       7,    6,    6,    6, 0x09,
      18,   16,    6,    6, 0x09,
      63,   16,    6,    6, 0x09,
     102,    6,    6,    6, 0x09,
     129,    6,    6,    6, 0x09,
     144,    6,    6,    6, 0x09,
     161,    6,    6,    6, 0x09,
     178,   16,    6,    6, 0x09,
     217,   16,    6,    6, 0x09,
     257,    6,    6,    6, 0x09,
     272,    6,    6,    6, 0x09,
     288,    6,    6,    6, 0x09,
     307,    6,    6,    6, 0x09,
     321,    6,    6,    6, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_query[] = {
    "query\0\0goExit()\0,\0"
    "queryListDoubleClicked(QTreeWidgetItem*,int)\0"
    "queryListClicked(QTreeWidgetItem*,int)\0"
    "textEditedQnameCb(QString)\0buttonSaveCb()\0"
    "buttonMkTextCb()\0buttonMkAdifCb()\0"
    "logListClickedCb(QTreeWidgetItem*,int)\0"
    "dataListClickedCb(QTreeWidgetItem*,int)\0"
    "buttonBackCb()\0buttonClearCb()\0"
    "buttonCopyDataCb()\0showHilfeCb()\0"
    "tabCurrentChangedCb(int)\0"
};

const QMetaObject query::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_query,
      qt_meta_data_query, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &query::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *query::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *query::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_query))
        return static_cast<void*>(const_cast< query*>(this));
    return QDialog::qt_metacast(_clname);
}

int query::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goExit(); break;
        case 1: queryListDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: queryListClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: textEditedQnameCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: buttonSaveCb(); break;
        case 5: buttonMkTextCb(); break;
        case 6: buttonMkAdifCb(); break;
        case 7: logListClickedCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: dataListClickedCb((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: buttonBackCb(); break;
        case 10: buttonClearCb(); break;
        case 11: buttonCopyDataCb(); break;
        case 12: showHilfeCb(); break;
        case 13: tabCurrentChangedCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
