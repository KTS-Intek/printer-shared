#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QStringList>
#include <QZXing.h>

class QrCodeGenerator
{

public:
    static QStringList getCorrection();

    static QString getCorrectionLetter(const int &indx);

    static QZXing::EncodeErrorCorrectionLevel correctionLevelFromLetter(const QString &letter);

    static QImage encodeImg(const QString &s, const QString &errcorr, const int &w, const int &h);

    static QImage encodeImg(const QString &s, const QString &errcorr, const QSize &sz);

    static bool canDecodeQr(const QImage &src, const QZXing::EncodeErrorCorrectionLevel &validCorrectionLevelEnum);

    static bool canDecodeQr(QZXing &decoder, const QImage &src);


    static QPixmap encode(const QString &s, const QString &errcorr, const int &w, const int &h);

    static QPixmap encode(const QString &s, const QString &errcorr, const QSize &sz);




    static QString decodeImage(const QImage &src, const bool &useFastTransformation, QString &mess);

    static QString decodeImage(QZXing &decoder, const QImage &src, const bool &useFastTransformation, QString &mess);

    static QString decodePixmap(const QPixmap &src, const bool &useFastTransformation, QString &mess);

    static QString decodePixmap(QZXing &decoder, const QPixmap &src, const bool &useFastTransformation, QString &mess);


    static QPixmap getPagePix(const int &wmm, const int &hmm, const int &dpi, const QString &corrlvl, const QString &s);

    static QPixmap getPagePixel(const int &borders, const int &whpxlsbrdrs, const int &wpxls, const int &hpxls, const QString &corrlvl, const QString &s);

};

#endif // QRCODEGENERATOR_H
