#include "qrdevicedecoder.h"
#include "src/printer/qrcodegeneratortypes.h"

QrDevParams QrDeviceDecoder::getDeviceParamFromQR(const QString &line, const bool &onlyDevType)
{
    const QStringList l = line.split("\n");

    QrDevParams dev;
    dev.isQrGood = false;
    dev.deviceType = QR_DEVTYPE_UNK;


    const int imax = l.size();
    if(imax > 3){
        int indxFrom = 0;
        if(l.first().contains("KI-UC-BB")){
            dev.isQrGood = true;
            dev.deviceType = QR_DEVTYPE_UC;
        }else{

            const quint32 d = l.first().toUInt(&dev.isQrGood);
            if(!dev.isQrGood)
                return dev;
            dev.deviceType = d;
            indxFrom = 1;
        }
        if(onlyDevType)
            return dev;

        QrDecodeMap m;
        for(int i = 1; indxFrom < imax; indxFrom++, i++)
            m.insert(i, l.at(indxFrom));
        dev.params = m;
    }
    return dev;

}

QrDevParams QrDeviceDecoder::getDeviceParamFromQR(const QString &line)
{
    return getDeviceParamFromQR(line, false);
}

quint32 QrDeviceDecoder::getDeviceTypeFromQR(const QString &line, bool &isQrGood)
{
    const QrDevParams d = getDeviceParamFromQR(line, true);
    isQrGood = d.isQrGood;
    return d.deviceType;
}

quint32 QrDeviceDecoder::getDeviceTypeFromQR(const QString &line)
{
    bool ok;
    return getDeviceTypeFromQR(line, ok);
}

bool QrDeviceDecoder::isUC(const QString &line)
{
    return (getDeviceTypeFromQR(line) == QR_DEVTYPE_UC);
}

bool QrDeviceDecoder::isEM(const QString &line)
{
    return (getDeviceTypeFromQR(line) == QR_DEVTYPE_EM);
}

bool QrDeviceDecoder::isWM(const QString &line)
{
    return (getDeviceTypeFromQR(line) == QR_DEVTYPE_WM);
}

bool QrDeviceDecoder::isLedController(const QString &line)
{
    return (getDeviceTypeFromQR(line) == QR_DEVTYPE_LED);
}

bool QrDeviceDecoder::isEMB(const QString &line)
{
    return (getDeviceTypeFromQR(line) == QR_DEVTYPE_EMB);
}

QString QrDeviceDecoder::getLedControllerNI(const QString &line)
{
    const QrDevParams m = getDeviceParamFromQR(line, false);
    return (m.deviceType == QR_DEVTYPE_LED) ? m.params.value(3) : QString();
}
