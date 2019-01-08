#ifndef QRCODEGENERATORTYPES_H
#define QRCODEGENERATORTYPES_H

#include <QList>
#include <QMap>
#include <QString>

#define QR_DEVTYPE_UC   0
#define QR_DEVTYPE_EM   1
#define QR_DEVTYPE_WM   2
#define QR_DEVTYPE_LED  3
#define QR_DEVTYPE_EMB  4
#define QR_DEVTYPE_CLBRDCONTENT 5//UCon connection settings

#define QR_DEVTYPE_UNK  0xFFFFFFFF


typedef QMap<QString,QString> QrTableMap;
typedef QList<QrTableMap> QrList;
typedef QMap<int,QString> QrTableColumnMap;//columns to QrHash keys

struct QrCodeGenSett
{
    quint32 qrDevType;
    QrTableColumnMap tableColumnMap;
    QrList list;
    quint8 copies2print;

    QrCodeGenSett() : qrDevType(QR_DEVTYPE_UNK), copies2print(1) {}
};


#endif // QRCODEGENERATORTYPES_H
