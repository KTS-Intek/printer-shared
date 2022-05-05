#include "qrcodegenerator.h"
#include <QPainter>
#include <QDebug>


QStringList QrCodeGenerator::getCorrection()
{
    return QString("L M Q H").split(" ", QString::SkipEmptyParts);
}

QString QrCodeGenerator::getCorrectionLetter(const int &indx)
{
    if(indx < 0 || indx > 3)
        return getCorrection().at(2);
    return getCorrection().at(indx);
}

QZXing::EncodeErrorCorrectionLevel QrCodeGenerator::correctionLevelFromLetter(const QString &letter)
{
    QZXing::EncodeErrorCorrectionLevel correctionLevelEnum = QZXing::EncodeErrorCorrectionLevel_M;
    if(letter == "H")
        correctionLevelEnum = QZXing::EncodeErrorCorrectionLevel_H;
    else if(letter == "Q")
        correctionLevelEnum = QZXing::EncodeErrorCorrectionLevel_Q;
    else if(letter == "M")
        correctionLevelEnum = QZXing::EncodeErrorCorrectionLevel_M;
    else if(letter == "L")
        correctionLevelEnum = QZXing::EncodeErrorCorrectionLevel_L;
//sz pixelW x pixelH
    return correctionLevelEnum;
}



QImage QrCodeGenerator::encodeImg(const QString &s, const QString &errcorr, const int &w, const int &h)
{
    return encodeImg(s, errcorr, QSize(w, h));
}

QImage QrCodeGenerator::encodeImg(const QString &s, const QString &errcorr, const QSize &sz)
{

    QZXing decoder(QZXing::DecoderFormat_QR_CODE, 0);
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE );
//    qDebug() << "encode " << errcorr << s;


    QZXingEncoderConfig encoderConfig(QZXing::EncoderFormat_QR_CODE, sz, correctionLevelFromLetter(errcorr), false, false);


    const QImage srcDef = QZXing::encodeData(s, encoderConfig);// QZXing::EncoderFormat_QR_CODE, sz, correctionLevelFromLetter(errcorr));
    if(canDecodeQr(decoder, srcDef))
        return srcDef;


//sz pixelW x pixelH
    const QStringList l = getCorrection();
    for(int i = 0, imax = l.size(); i < imax; i++){
//        qDebug() << "encode " << i << l.at(i) << s;

        encoderConfig.errorCorrectionLevel = correctionLevelFromLetter(l.at(i));
        const QImage src = QZXing::encodeData(s, encoderConfig);// QZXing::EncoderFormat_QR_CODE, sz, correctionLevelFromLetter(l.at(i)));
        if(canDecodeQr(decoder, src))
            return src;
    }


    return QImage();// QZXing::encodeData(s, QZXing::EncoderFormat_QR_CODE, sz, correctionLevelFromLetter(errcorr));
}


bool QrCodeGenerator::canDecodeQr(QZXing &decoder, const QImage &src)
{
    QString messageStrr;
    const QString d = decodeImage(decoder, src, true, messageStrr);
//    qDebug() << "checkdecode " << d << messageStrr;
    return !d.isEmpty();
}

QPixmap QrCodeGenerator::encode(const QString &s, const QString &errcorr, const int &w, const int &h)
{
    return encode(s, errcorr, QSize(w, h));
}

QPixmap QrCodeGenerator::encode(const QString &s, const QString &errcorr, const QSize &sz)
{
    return QPixmap::fromImage(encodeImg(s, errcorr, sz), Qt::MonoOnly);
}


QString QrCodeGenerator::decodeImage(const QImage &src, const bool &useFastTransformation, QString &messageStrr)
{
    QZXing decoder(QZXing::DecoderFormat_QR_CODE, 0);
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE );
    return decodeImage(decoder, src, useFastTransformation, messageStrr);
}

QString QrCodeGenerator::decodeImage(QZXing &decoder, const QImage &src, const bool &useFastTransformation, QString &messageStrr)
{
    const QString s = decoder.decodeImage(src, -1, -1, !useFastTransformation );
    if(s.isEmpty())
        messageStrr = "no qr";// decoder.getLastError();
    return s;
}

QString QrCodeGenerator::decodePixmap(const QPixmap &src, const bool &useFastTransformation, QString &messageStrr)
{
    QZXing decoder(QZXing::DecoderFormat_QR_CODE, 0);
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE );
    return decodePixmap(decoder, src, useFastTransformation, messageStrr);
}

QString QrCodeGenerator::decodePixmap(QZXing &decoder, const QPixmap &src, const bool &useFastTransformation, QString &messageStrr)
{
    return decodeImage(decoder, src.toImage(), useFastTransformation, messageStrr);
}

QPixmap QrCodeGenerator::getPagePix(const PrintImageHelper::PrintSettCache &printSett, const QString &s)
{
    return getPagePix(printSett.widthMM, printSett.heightMM, printSett.resolutionDpi, printSett.qrCorrLetter, s);
}

QPixmap QrCodeGenerator::getPagePix(const int &wmm, const int &hmm, const int &dpi, const QString &corrlvl, const QString &s)
{
    const int whpxls = qMax(1, int( qreal(qMin(wmm, hmm) * dpi) / qreal(25.4)));
    const int borders = whpxls * 0.05;
    const int whpxlsbrdrs = whpxls - borders * 2;

    const int wpxls = int(qreal(wmm * dpi) / qreal(25.4));
    const int hpxls = int(qreal(hmm * dpi) / qreal(25.4));

//    const int whpxls = qMin(ui->sbWidth->value(), ui->sbHeight->value()) * dpmm;


    return getPagePixel(borders, whpxlsbrdrs, wpxls, hpxls, corrlvl, s);

}

QPixmap QrCodeGenerator::getPagePixel(const int &borders, const int &whpxlsbrdrs, const int &wpxls, const int &hpxls, const QString &corrlvl, const QString &s)
{

    return getPagePixel(borders, borders, whpxlsbrdrs, wpxls, hpxls, corrlvl, s);
}

QPixmap QrCodeGenerator::getPagePixel(const int &left, const int &top, const int &whpxlsbrdrs, const int &wpxls, const int &hpxls, const QString &corrlvl, const QString &s)
{
    const QPixmap p = encode(s, corrlvl, whpxlsbrdrs, whpxlsbrdrs);//.scaled(wpxls, hpxls, Qt::KeepAspectRatio);
//    white.swap(p);

    QPixmap white = QPixmap(wpxls, hpxls);
    white.fill(QColor(Qt::white));
    QPainter painter(&white);
    painter.drawPixmap(left, top, p);
    return  white;
}
