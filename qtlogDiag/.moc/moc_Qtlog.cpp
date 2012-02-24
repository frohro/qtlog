/****************************************************************************
** Meta object code from reading C++ file 'Qtlog.h'
**
** Created: Fri Feb 24 14:36:17 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Qtlog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Qtlog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtLog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      52,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x09,
      16,    6,    6,    6, 0x09,
      25,    6,    6,    6, 0x09,
      35,    6,    6,    6, 0x09,
      45,    6,    6,    6, 0x09,
      64,    6,    6,    6, 0x09,
      82,    6,    6,    6, 0x09,
     100,    6,    6,    6, 0x09,
     118,    6,    6,    6, 0x09,
     135,    6,    6,    6, 0x09,
     153,    6,    6,    6, 0x09,
     165,    6,    6,    6, 0x09,
     178,    6,    6,    6, 0x09,
     187,    6,    6,    6, 0x09,
     202,    6,    6,    6, 0x09,
     213,    6,    6,    6, 0x09,
     226,    6,    6,    6, 0x09,
     243,    6,    6,    6, 0x09,
     251,    6,    6,    6, 0x09,
     268,    6,    6,    6, 0x09,
     281,    6,    6,    6, 0x09,
     296,    6,    6,    6, 0x09,
     311,    6,    6,    6, 0x09,
     328,    6,    6,    6, 0x09,
     343,    6,    6,    6, 0x09,
     356,    6,    6,    6, 0x09,
     368,    6,    6,    6, 0x09,
     380,    6,    6,    6, 0x09,
     394,    6,    6,    6, 0x09,
     416,    6,    6,    6, 0x09,
     429,    6,    6,    6, 0x09,
     447,    6,    6,    6, 0x09,
     466,    6,    6,    6, 0x09,
     476,    6,    6,    6, 0x09,
     488,    6,    6,    6, 0x09,
     505,    6,    6,    6, 0x09,
     514,    6,    6,    6, 0x09,
     528,    6,    6,    6, 0x09,
     545,    6,    6,    6, 0x09,
     565,    6,    6,    6, 0x09,
     583,    6,    6,    6, 0x09,
     608,    6,    6,    6, 0x09,
     622,    6,    6,    6, 0x09,
     637,    6,    6,    6, 0x09,
     653,    6,    6,    6, 0x09,
     674,    6,    6,    6, 0x09,
     710,    6,    6,    6, 0x09,
     743,    6,    6,    6, 0x09,
     757,    6,    6,    6, 0x09,
     774,    6,    6,    6, 0x09,
     806,    6,    6,    6, 0x08,
     833,  831,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtLog[] = {
    "QtLog\0\0goExit()\0addLog()\0addClog()\0"
    "addDlog()\0awdDxStatusSsbAc()\0"
    "awdDxStatusCwAc()\0awdJahresModeAc()\0"
    "awdJahresBandAc()\0awdDLDstatusAc()\0"
    "awdIOTAstatusAc()\0logconfAc()\0"
    "standortAc()\0clubAc()\0InternetOnAc()\0"
    "rigCtlAc()\0confmodeAc()\0verbindungenAc()\0"
    "rigAc()\0reportLayoutAc()\0dataSaveAc()\0"
    "adifExportAc()\0adifImportAc()\0"
    "queryTextoutAc()\0qslEingangAc()\0"
    "qslDruckAc()\0admindbAc()\0dxTableAc()\0"
    "prefTableAc()\0ruDistrictAwdListAc()\0"
    "qrzTableAc()\0dokVerwaltungAc()\0"
    "iotaVerwaltungAc()\0notizAc()\0VersionAc()\0"
    "RigCtlHamlibAc()\0InfoAc()\0checkReport()\0"
    "checkBoxSortCb()\0boxLimitCb(QString)\0"
    "groupEditLineCb()\0groupEditLineCb(QString)\0"
    "boxAwdCb(int)\0boxSortCb(int)\0"
    "awdEditLineCb()\0boxLayoutCb(QString)\0"
    "updateReportItem(QTableWidgetItem*)\0"
    "updateAwdItem(QTableWidgetItem*)\0"
    "removeQsoCb()\0checkBoxDateCb()\0"
    "checkAwdItem(QTableWidgetItem*)\0"
    "processPendingDatagram()\0,\0"
    "processWriteDatagram(int,QString)\0"
};

const QMetaObject QtLog::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtLog,
      qt_meta_data_QtLog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtLog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtLog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtLog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtLog))
        return static_cast<void*>(const_cast< QtLog*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtLog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: goExit(); break;
        case 1: addLog(); break;
        case 2: addClog(); break;
        case 3: addDlog(); break;
        case 4: awdDxStatusSsbAc(); break;
        case 5: awdDxStatusCwAc(); break;
        case 6: awdJahresModeAc(); break;
        case 7: awdJahresBandAc(); break;
        case 8: awdDLDstatusAc(); break;
        case 9: awdIOTAstatusAc(); break;
        case 10: logconfAc(); break;
        case 11: standortAc(); break;
        case 12: clubAc(); break;
        case 13: InternetOnAc(); break;
        case 14: rigCtlAc(); break;
        case 15: confmodeAc(); break;
        case 16: verbindungenAc(); break;
        case 17: rigAc(); break;
        case 18: reportLayoutAc(); break;
        case 19: dataSaveAc(); break;
        case 20: adifExportAc(); break;
        case 21: adifImportAc(); break;
        case 22: queryTextoutAc(); break;
        case 23: qslEingangAc(); break;
        case 24: qslDruckAc(); break;
        case 25: admindbAc(); break;
        case 26: dxTableAc(); break;
        case 27: prefTableAc(); break;
        case 28: ruDistrictAwdListAc(); break;
        case 29: qrzTableAc(); break;
        case 30: dokVerwaltungAc(); break;
        case 31: iotaVerwaltungAc(); break;
        case 32: notizAc(); break;
        case 33: VersionAc(); break;
        case 34: RigCtlHamlibAc(); break;
        case 35: InfoAc(); break;
        case 36: checkReport(); break;
        case 37: checkBoxSortCb(); break;
        case 38: boxLimitCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 39: groupEditLineCb(); break;
        case 40: groupEditLineCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 41: boxAwdCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: boxSortCb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: awdEditLineCb(); break;
        case 44: boxLayoutCb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 45: updateReportItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 46: updateAwdItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 47: removeQsoCb(); break;
        case 48: checkBoxDateCb(); break;
        case 49: checkAwdItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 50: processPendingDatagram(); break;
        case 51: processWriteDatagram((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 52;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
