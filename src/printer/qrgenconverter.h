#ifndef QRGENCONVERTER_H
#define QRGENCONVERTER_H

#include <QStringList>


///[!] printer-shared
#include "qrcodegeneratortypes.h"


class QrGenConverter
{
public:
    static void setupQrCodeGet(QrCodeGenSett &qrcodegensett, const int &qrDevType, const QrTableColumnMap &tableMap);

    static void setupQrCodeGet(QrCodeGenSett &qrcodegensettall, QrCodeGenSett &qrcodegensettsel, const int &qrDevType, const QList<int> &lk4cols);

    static QHash<QString,QString> keys2human();

    static QVariantMap getMapAboutDev(const QrTableMap &tableMap);

    static QStringList keys2human(const QStringList &lk);

    static QString getDeviceLine(const QrTableMap &tableMap, const int &qrDevType);

    static QString getDeviceLine(const QrTableMap &tableMap, const int &qrDevType, const QStringList &keys4dev);

    static QString getDeviceLine(const QrTableMap &tableMap, const int &qrDevType, const QStringList &keys4dev, const int &keys4devLen);

    static QStringList getDeviceLines(const QrList &tableList, const int &qrDevType, const int &copies);


    static QrTableColumnMap getTableMap4dev(const int &qrDevType, const QList<int> &lk4cols);

    static QStringList getAllKeys4dev();

    static QStringList getKeys4dev(const int &qrDevType);

    static QString getKeys4uc();

    static QString getKeys4em();

    static QString getKeys4wm();

    static QString getKeys4smartLed();

    static QString getKeys4emb();

    static QString getKeys4ucon();

    static QString getKeys4clbrdContent();

};

#endif // QRGENCONVERTER_H
