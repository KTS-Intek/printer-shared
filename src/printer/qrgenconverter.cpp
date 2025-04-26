#include "qrgenconverter.h"
#include <QObject>

void QrGenConverter::setupQrCodeGet(QrCodeGenSett &qrcodegensett, const int &qrDevType, const QrTableColumnMap &tableMap)
{
    qrcodegensett.list.clear();
    qrcodegensett.qrDevType = qrDevType;
    qrcodegensett.tableColumnMap = tableMap;
    qrcodegensett.copies2print = 1;
}

void QrGenConverter::setupQrCodeGet(QrCodeGenSett &qrcodegensettall, QrCodeGenSett &qrcodegensettsel, const int &qrDevType, const QList<int> &lk4cols)
{
    const QrTableColumnMap tableMap = getTableMap4dev(qrDevType, lk4cols);
    setupQrCodeGet(qrcodegensettall, qrDevType, tableMap);
    setupQrCodeGet(qrcodegensettsel, qrDevType, tableMap);
}

QHash<QString, QString> QrGenConverter::keys2human()
{
    const QStringList lk = getAllKeys4dev();//QString("model sn mac eui64 imei ni prof param")

    const QStringList lh = QObject::tr("Model;S/N;MAC;EUI64;IMEI;NI;Profile;Parameters").split(";");
    QHash<QString,QString> h;
    for(int i = 0, imax = qMin(lk.size(), lh.size()); i < imax; i++)
        h.insert(lk.at(i), lh.at(i));

    return h;
}

QVariantMap QrGenConverter::getMapAboutDev(const QrTableMap &tableMap)
{
    const QStringList lk = getAllKeys4dev();//QString("model sn mac eui64 imei ni prof param")
    QVariantMap m;
    for(int i = 0, imax =lk.size(); i < imax; i++){
        if(tableMap.contains(lk.at(i)))
            m.insert( "$" + lk.at(i), tableMap.value(lk.at(i)));

    }

    return m;
}

QStringList QrGenConverter::keys2human(const QStringList &lk)
{
    const QHash<QString,QString> h = keys2human();
    QStringList l;
    for(int i = 0, imax = lk.size(); i < imax; i++)
        l.append(h.value(lk.at(i), lk.at(i)));
    return l;
}

QString QrGenConverter::getDeviceLine(const QrTableMap &tableMap, const int &qrDevType)
{
    return getDeviceLine(tableMap, qrDevType, getKeys4dev(qrDevType));
}

QString QrGenConverter::getDeviceLine(const QrTableMap &tableMap, const int &qrDevType, const QStringList &keys4dev)
{
    return getDeviceLine(tableMap, qrDevType, keys4dev, keys4dev.size());
}

QString QrGenConverter::getDeviceLine(const QrTableMap &tableMap, const int &qrDevType, const QStringList &keys4dev, const int &keys4devLen)
{
    if(tableMap.isEmpty())
        return QString();

    QStringList l;
    for(int i = 0; i < keys4devLen; i++)
        l.append(tableMap.value(keys4dev.at(i)));
    if(!l.isEmpty())
        l.prepend(QString::number(qrDevType));
    return l.join("\n");
}



QStringList QrGenConverter::getDeviceLines(const QrList &tableList, const int &qrDevType, const int &copies)
{
    QStringList l;
    const QStringList devKeys = getKeys4dev(qrDevType);
    const int devKeysLen = devKeys.size();
    for(int i = 0, imax = tableList.size(); i < imax; i++){
        const QString s = getDeviceLine(tableList.at(i), qrDevType, devKeys, devKeysLen);
        if(s.isEmpty())
            continue;
        for(int c = 0; c < copies; c++)
            l.append(s);
    }
    return l;
}

QrTableColumnMap QrGenConverter::getTableMap4dev(const int &qrDevType, const QList<int> &lk4cols)
{
    const QStringList lk = getKeys4dev(qrDevType);

    const int lksize = lk.size();
    const int lkcolss = lk4cols.size();

    if(lksize != lkcolss || lk.isEmpty())
        return QrTableColumnMap();

    QrTableColumnMap tableColumnMap;

    int lastMinCol = -1;
    for(int i = 0, imax = lk.size(); i < imax; i++){
       int col = lk4cols.at(i);
       if(col < 0){
           col = lastMinCol;
           lastMinCol--;
       }
        tableColumnMap.insert(col, lk.at(i));
    }

    return tableColumnMap;
}

QStringList QrGenConverter::getAllKeys4dev()
{
    return QString("model sn mac eui64 imei ni prof param type name version value").split(" ", Qt::SkipEmptyParts);
}

QStringList QrGenConverter::getKeys4dev(const int &qrDevType)
{
    QString s;
    switch(qrDevType){
    case QR_DEVTYPE_UC  : s = getKeys4uc()      ; break;
    case QR_DEVTYPE_EM  : s = getKeys4em()      ; break;
    case QR_DEVTYPE_WM  : s = getKeys4wm()      ; break;
    case QR_DEVTYPE_LED : s = getKeys4smartLed(); break;
    case QR_DEVTYPE_EMB : s = getKeys4emb()     ; break;
//    case QR_DEVTYPE_UCON: s = getKeys4ucon()    ; break;
    case QR_DEVTYPE_CLBRDCONTENT: getKeys4clbrdContent(); break;

    }
    return s.split(" ", Qt::SkipEmptyParts);
}

QString QrGenConverter::getKeys4uc()
{
    return "model sn mac eui64 imei";
}

QString QrGenConverter::getKeys4em()
{
    return "model sn ni";
}

QString QrGenConverter::getKeys4wm()
{
    return "model sn ni prof";
}

QString QrGenConverter::getKeys4smartLed()
{
    return "model sn ni";
}

QString QrGenConverter::getKeys4emb()
{
    return "model eui64 param";
}

QString QrGenConverter::getKeys4ucon()
{
    return "pas l useMac objN a p mac t cmMAC";
}

QString QrGenConverter::getKeys4clbrdContent()
{
    return "type name version value";
}
