#ifndef QRDEVICEDECODER_H
#define QRDEVICEDECODER_H

#include <QStringList>
#include <QMap>



typedef QMap<int,QString> QrDecodeMap;

struct QrDevParams
{
    bool isQrGood;
    quint32 deviceType;
    QrDecodeMap params;
    QrDevParams() {}
    QrDevParams(const bool &isQrGood, const quint32 &deviceType, const QrDecodeMap &params) : isQrGood(isQrGood), deviceType(deviceType), params(params) {}
};

class QrDeviceDecoder
{
public:
    static QrDevParams getDeviceParamFromQR(const QString &line, const bool &onlyDevType);

    static QrDevParams getDeviceParamFromQR(const QString &line);

    static quint32 getDeviceTypeFromQR(const QString &line, bool &isQrGood);

    static quint32 getDeviceTypeFromQR(const QString &line);


    static bool isUC(const QString &line);

    static bool isEM(const QString &line);

    static bool isWM(const QString &line);

    static bool isLedController(const QString &line);

    static bool isEMB(const QString &line);


    static QString getLedControllerNI(const QString &line);

};

#endif // QRDEVICEDECODER_H
