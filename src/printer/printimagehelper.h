#ifndef PRINTIMAGEHELPER_H
#define PRINTIMAGEHELPER_H


#include <QPixmap>
#include <QImage>
#include <QStringList>
#include <QPrinter>

class PrintImageHelper
{

public:

    struct PrintSettCache{
        int widthMM;
        int heightMM;
        int resolutionDpi;
        bool isPortrait;

        int left;
        int top;
        int right;
        int bottom;

        int contrast;
        int brightness;

        QString path2img;
        QString userData;
        int rotateDeg;
        int formatIndx;
        int colorIndx;
        int fontPpt;
        int textTopMargin;
        int textLeftMargin;
        qreal textRotateDeg;

        QString qrCorrLetter;

        QString dateMask;
    };

    static QStringList getSupportedImageFormat();

    static QStringList getSupportedImageColorConv();

    static QStringList getSupportedDateMask();


    static QPixmap getTemplateImage(const QString defaultPath = "template/label.png");

    static QString replaceKeysInText(QString plainText, QString dateMask, const QString eui64 = "0123456789ABCDEF");

    static QString replaceKeysInText(QString plainText, QString dateMask, const QVariantMap &about);


    static QPixmap rotateImage(const int &rotateDeg, const QPixmap &pm);

    static QPixmap convertImage(const int &imgFormat, const int &imgConvflags, const QPixmap &pm, const int contrast = 50, const int brightness = 50);

    static QPixmap getPixmapWithUserData(const QString &userString, const QString &userDateMask, const int &userFontPpt, const int &topMarginPx, const int &leftMarginPx, const QString eui64 = "0123456789ABCDEF", const QString defaultPath = "template/label.png");

    static QPixmap getPixmapWithUserData(const QString &userString, const QString &userDateMask, const int &userFontPpt, const int &topMarginPx, const int &leftMarginPx, const QVariantMap &about, const qreal &rotate, QPixmap p);

    static QString printPixmap(const QString &printerName, const QString &pdfFileName, const int &copies, const int &resolutionDpi, const QSizeF pageSizeF, const bool &isPortrait, const QMargins &margins, const QPixmap &p);

    static QString printPixmaps(const QString &printerName, const QString &pdfFileName, const int &copies, const int &resolutionDpi, const QSizeF pageSizeF, const bool &isPortrait, const QMargins &margins, const QList<QPixmap> &lp);




    static QImage makeContrast(const QImage &prev_image, const int &contrast);

    static QImage makeBrightness(const QImage &img, const int &brightness);

    static QImage makeContrastV2(const QImage &img, const int &contrast);

    static QSizeF getPageSize(const int &w, const int &h, const bool &isPortrait);


    static PrintSettCache defaultPrintSett();

    static PrintSettCache variantMap2printSett(const QVariantMap &m);

    static QVariantMap printSett2variantMap(const PrintSettCache &printSett);

    static qreal calculateDevicePixelRatio(const int &zoom, const int &minimum, const int &maximum);
};

#endif // PRINTIMAGEHELPER_H
