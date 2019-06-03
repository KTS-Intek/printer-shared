#include "printimagehelper.h"
#include <QDate>
#include <QFileInfo>
#include <QMatrix>
#include <QPainter>
#include <QFont>
#include <QImage>
#include <QApplication>
#include <QDesktopServices>
#include <QPaintEngine>
#include <QUrl>
#include <QPrinter>
#include <QPrinterInfo>
#include <QDebug>
#include <QThread>


///[!] type-converter
#include "src/base/valuevalidator.h"

//------------------------------------------------------------------------------------------------

QStringList PrintImageHelper::getSupportedImageFormat()
{
    QStringList l;
    l.append(QString("Do not change"));
    l.append(QString("Mono"));
    l.append(QString("MonoLSB"));
    l.append(QString("Indexed8"));
    l.append(QString("RGB32"));
    l.append(QString("ARGB32"));
    l.append(QString("ARGB32_Premultiplied"));
    l.append(QString("RGB16"));
    l.append(QString("ARGB8565_Premultiplied"));
    l.append(QString("RGB666"));
    l.append(QString("ARGB6666_Premultiplied"));
    l.append(QString("RGB555"));
    l.append(QString("ARGB8555_Premultiplied"));
    l.append(QString("RGB888"));
    l.append(QString("RGB444"));
    l.append(QString("ARGB4444_Premultiplied"));
    l.append(QString("RGBX8888"));
    l.append(QString("RGBA8888"));
    l.append(QString("RGBA8888_Premultiplied"));
    l.append(QString("BGR30"));
    l.append(QString("A2BGR30_Premultiplied"));
    l.append(QString("RGB30"));
    l.append(QString("A2RGB30_Premultiplied"));
    l.append(QString("Alpha8"));
    l.append(QString("Grayscale8"));
    return l;
}

//------------------------------------------------------------------------------------------------

QStringList PrintImageHelper::getSupportedImageColorConv()
{
    QStringList l;
    l.append(QString("AutoColor"));
    l.append(QString("ColorOnly"));
    l.append(QString("MonoOnly"));
    return l;
}

//------------------------------------------------------------------------------------------------

QStringList PrintImageHelper::getSupportedDateMask()
{
    return QString("dd-MM-yyyy;yyyy-MM-dd").split(";");
}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::getTemplateImage(const QString defaultPath)
{
    return getTemplateImageExt("", defaultPath);
}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::getTemplateImageExt(const QString &path2img, const QString &defaultPath)
{
    if(!path2img.isEmpty()){
        QFileInfo fi(path2img);
        if(fi.exists() && fi.isReadable())
            return QPixmap(path2img);
    }

    if(!defaultPath.isEmpty()){
        QFileInfo fi(defaultPath);
        if(fi.exists() && fi.isReadable())
            return QPixmap(defaultPath);
    }
    return QPixmap(":/katynko/label-print-def.png");


}

//------------------------------------------------------------------------------------------------

QString PrintImageHelper::replaceKeysInText(QString plainText, QString dateMask, const QString eui64)
{

    QVariantMap map;
    map.insert("$eui64", eui64);
    return replaceKeysInText(plainText, dateMask, map);
}

//------------------------------------------------------------------------------------------------

QString PrintImageHelper::replaceKeysInText(QString plainText, QString dateMask, QVariantMap about)
{
    /*
     * keys
     * $date
     * $eui64
     * $ni
     * $model
     * $lcuni - it generates from eui64
     */

    if(plainText.contains("$date"))
        plainText.replace("$date", QDate::currentDate().toString(dateMask));

    const QStringList l = QString("$ni $eui64 $model $lcuni").split(" ", QString::SkipEmptyParts);

    if(!about.contains("$lcuni")){
        bool ok;
        const quint64 lcuni = about.value("$eui64").toString().right(8).toULongLong(&ok,16);
        const QString s = QString::number(lcuni, 16);
        if(ok && !s.isEmpty())
            about.insert("$lcuni", s);
    }

    for(int i = 0, imax = l.size(); i < imax; i++){
        if(plainText.contains(l.at(i)) && about.contains(l.at(i)))
            plainText.replace(l.at(i), about.value(l.at(i)).toString());
    }
    return plainText;
}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::rotateImage(const int &rotateDeg, const QPixmap &pm)
{

    QImage img = pm.toImage();// p.toImage().convertToFormat(QImage::Format_Grayscale8, Qt::MonoOnly);

    QPoint center = img.rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(rotateDeg);
    return QPixmap::fromImage(img.transformed(matrix));
}

QImage::Format PrintImageHelper::getImgFormat(const int &imgFormat)
{
    QImage::Format format = QImage::Format_Invalid;

    switch(imgFormat){

    case  1: format = QImage::Format_Mono                   ; break;
    case  2: format = QImage::Format_MonoLSB                ; break;
    case  3: format = QImage::Format_Indexed8               ; break;
    case  4: format = QImage::Format_RGB32                  ; break;
    case  5: format = QImage::Format_ARGB32                 ; break;
    case  6: format = QImage::Format_ARGB32_Premultiplied   ; break;
    case  7: format = QImage::Format_RGB16                  ; break;
    case  8: format = QImage::Format_ARGB8565_Premultiplied ; break;
    case  9: format = QImage::Format_RGB666                 ; break;
    case 10: format = QImage::Format_ARGB6666_Premultiplied ; break;
    case 11: format = QImage::Format_RGB555                 ; break;
    case 12: format = QImage::Format_ARGB8555_Premultiplied ; break;
    case 13: format = QImage::Format_RGB888                 ; break;
    case 14: format = QImage::Format_RGB444                 ; break;
    case 15: format = QImage::Format_ARGB4444_Premultiplied ; break;
    case 16: format = QImage::Format_RGBX8888               ; break;
    case 17: format = QImage::Format_RGBA8888               ; break;
    case 18: format = QImage::Format_RGBA8888_Premultiplied ; break;
    case 19: format = QImage::Format_BGR30                  ; break;
    case 20: format = QImage::Format_A2BGR30_Premultiplied  ; break;
    case 21: format = QImage::Format_RGB30                  ; break;
    case 22: format = QImage::Format_A2RGB30_Premultiplied  ; break;
    case 23: format = QImage::Format_Alpha8                 ; break;
    case 24: format = QImage::Format_Grayscale8             ; break;

    default: format = QImage::Format_Invalid                ; break;
    }
    return format;
}

//------------------------------------------------------------------------------------------------

Qt::ImageConversionFlags PrintImageHelper::getConversionFlags(const int &imgConvflags)
{
    Qt::ImageConversionFlags flags;
    switch(imgConvflags){
    case 1: flags = Qt::ColorOnly   ; break;
    case 2: flags = Qt::MonoOnly    ; break;

    default: flags = Qt::AutoColor  ; break;
    }
    return flags;
}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::convertImage(const int &imgFormat, const int &imgConvflags, const QPixmap &pm, const int contrast, const int brightness)
{
    const Qt::ImageConversionFlags flags = getConversionFlags(imgConvflags);

    const QImage::Format format = getImgFormat(imgFormat);


    if(format == QImage::Format_Invalid)
        return pm;

//    qDebug() << "img1 " << img.devicePixelRatioF() << img.depth() << img.dotsPerMeterX() << img.dotsPerMeterY() << img.height() << img.heightMM() << img.width() << img.widthMM();

    QImage img = pm.toImage().convertToFormat(format, flags);

    img = makeBrightness(img, brightness);
    img = makeContrast(img, contrast);

////   img.setDevicePixelRatio(2);
//    img.setDotsPerMeterX(dpi/0.0254);  //inch to meter (dpi * 1/0.0254);
//    img.setDotsPerMeterY(dpi/0.0254);

//   qDebug() << "img2 " << img.devicePixelRatioF() << img.depth() << img.dotsPerMeterX() << img.dotsPerMeterY() << img.height() << img.heightMM() << img.width() << img.widthMM();
////   const double dpiRatio = (double)img.dotsPerMeterY() / (double)img.dotsPerMeterX();

////   QPixmap pm2 = QPixmap::fromImage(img.scaled(
////                                        (dpiRatio > 1.0) ? img.width() : (int)(img.width() * dpiRatio),
////                                        (dpiRatio < 1.0) ? img.height() : (int)(img.height() / dpiRatio),
////                                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation
////                                    ));

//   QPixmap pm2(img.size());
//   QPainter painter(&pm2);

//   painter.drawImage(0,0,img);

//   qDebug() << "img3 " << pm2.devicePixelRatio() << pm2.depth() << pm2.height() << pm2.heightMM() << pm2.width() << pm2.widthMM();
   return QPixmap::fromImage(img, flags);


}

//------------------------------------------------------------------------------------------------

QVariantMap PrintImageHelper::getMapAboutDev(const QString &euiline, const QString &euilinekeys, const QString &euilineslitsymb)
{
    const QStringList l = euiline.split(euilineslitsymb);
    const QStringList keys = euilinekeys.split(" ", QString::SkipEmptyParts);

    QVariantMap map;
    for(int i = 0, imax = l.size(), imax2 = keys.size(); i < imax && i < imax2; i++){
        if(l.at(i).isEmpty())
            continue;
        map.insert(keys.at(i), l.at(i));
    }

    return map;
}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::getPixmapWithUserDataExt(const PrintImageHelper::PrintSettCache &printSett, const QString &euiline, const QString &euilinekeys, const QString &euilineslitsymb, const QPixmap &qrimg)
{
    const QVariantMap map = getMapAboutDev(euiline, euilinekeys, euilineslitsymb);

    QPixmap background = getTemplateImageExt(printSett.usedefaultimage ? "" : printSett.backgroundimagepath, printSett.defaultimagepath);

    if(printSett.genearateaqrcode && !qrimg.isNull()){//a QR code processing part
        QPainter painter(&background);

        const Qt::ImageConversionFlags flags = getConversionFlags(printSett.colorIndx);
        const QImage::Format format = getImgFormat(printSett.formatIndx);

        QImage img = qrimg.toImage().convertToFormat(format, flags);

        img = makeBrightness(img, printSett.brightness);
        img = makeContrast(img, printSett.contrast);

        if(printSett.enqrcustomposition)
            painter.drawImage(QRect(printSett.qrleftmarginpx, printSett.qrtopmarginpx, printSett.qrwidthpx, printSett.qrheightpx), img);
        else
            painter.drawImage(0, 0, img);
    }

    return getPixmapWithUserData(printSett.userData, printSett.dateMask, printSett.fontPpt, printSett.textTopMargin, printSett.textLeftMargin, map, 0.0, background);

}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::getPixmapWithUserData(const QString &userString, const QString &userDateMask, const int &userFontPpt, const int &topMarginPx, const int &leftMarginPx, const QString eui64, const QString defaultPath)
{

    QVariantMap map;
    map.insert("$eui64", eui64);//it is an old method, use getPixmapWithUserDataExt instead



    return getPixmapWithUserData(userString, userDateMask, userFontPpt, topMarginPx, leftMarginPx, map, 0.0, getTemplateImage(defaultPath));

}

//------------------------------------------------------------------------------------------------

QPixmap PrintImageHelper::getPixmapWithUserData(const QString &userString, const QString &userDateMask, const int &userFontPpt, const int &topMarginPx, const int &leftMarginPx, const QVariantMap &about, const qreal &rotate, QPixmap p)
{
    QPainter painter(&p);
    QFont f = painter.font();
    f.setPointSize(userFontPpt);
    painter.setFont(f);

    const int w = p.width();
    const int h = p.height();

    const int x = leftMarginPx;
    const int y = topMarginPx ;
    if(rotate != 0.0)
        painter.rotate(rotate);
    painter.drawText( QRect(x, y, w , h ), replaceKeysInText(userString, userDateMask, about) );
    painter.end();

    return p;
}
//------------------------------------------------------------------------------------------------
QString PrintImageHelper::printPixmap(const QString &printerName, const QString &pdfFileName, const int &copies, const int &resolutionDpi, const QSizeF pageSizeF, const bool &isPortrait, const QMargins &margins, const QPixmap &p)
{
    return printPixmaps(printerName, pdfFileName, copies, resolutionDpi, pageSizeF, isPortrait, margins, QList<QPixmap>() << p);

}
//------------------------------------------------------------------------------------------------
QString PrintImageHelper::printPixmaps(const QString &printerName, const QString &pdfFileName, const int &copies, const int &resolutionDpi, const QSizeF pageSizeF, const bool &isPortrait, const QMargins &margins, const QList<QPixmap> &lp)
{
    //QPrinter &printer
    QPrinter printer(QPrinterInfo::printerInfo(printerName), QPrinter::HighResolution); // const QString &printerName,


    printer.setFullPage(true);
    printer.setResolution(resolutionDpi);
    printer.setPageSize(QPageSize(pageSizeF, QPageSize::Millimeter));
    printer.setPageMargins(margins);

    printer.setOrientation(isPortrait ? QPrinter::Portrait : QPrinter::Landscape);
    printer.setPageOrientation(isPortrait ? QPageLayout::Portrait : QPageLayout::Landscape);


    const bool usePdfFormat = (printerName.isEmpty());

    if(!usePdfFormat && !printer.isValid())
        return QString("The printer is invalid(");

    if(usePdfFormat){
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(pdfFileName);

    }

    QPainter painter;

    painter.begin(&printer);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    bool iNeedNewPage = false;
    for(int i = 0, imax = lp.size(); i < imax; i++){
        const QPixmap p = lp.at(i);

        for(int c = 0; c < copies; c++){

            if(iNeedNewPage)
                printer.newPage();
            else
                iNeedNewPage = true;

            printer.paintEngine()->drawPixmap( QRectF(0,0,p.width(),p.height()), p, QRectF(0,0,p.width(),p.height()) );

        }

    }

    painter.end();

     if(usePdfFormat){
#ifdef Q_OS_WIN
        QDesktopServices::openUrl(QUrl("file:///" + pdfFileName));
#else
        QDesktopServices::openUrl(QUrl("file://" + pdfFileName));
#endif
     }
     return "";
}
//------------------------------------------------------------------------------------------------
QImage PrintImageHelper::makeContrast(const QImage &prev_image, const int &contrast)
{


    QImage imageToProcess = prev_image;// QImage(prev_image.size(), format);

    if(contrast != 0){
        qreal factor = (259.0 * ((qreal)contrast + 255.0)) / (255.0 * (259.0 - (qreal)contrast));

        int r = 0, g = 0, b = 0;
        for (int y = 0; y < prev_image.height(); ++y) {
            for (int x = 0; x < prev_image.width(); ++x) {
                QColor col = prev_image.pixelColor(x, y);
                col.getRgb(&r,&g,&b);
                r = factor * (r - 128) + 128;
                g = factor * (g - 128) + 128;
                b = factor * (b - 128) + 128;


                if(r>255)
                    r=255;
                else if(r < 0)
                    r = 0;
                if(g>255)
                    g=255;
                else if(g < 0)
                    g = 0;
                if(b>255)
                    b=255;
                else if(b < 0)
                    b = 0;
                imageToProcess.setPixelColor(x,y, QColor(r,g,b));
            }
        }
        return imageToProcess;
    }
    return prev_image;


}
//------------------------------------------------------------------------------------------------
QImage PrintImageHelper::makeBrightness(const QImage &img, const int &brightness)
{
    if(brightness == 0)
        return img;
    //brightness 0-100%
    QImage image = img;// QImage(img.size(), format);
//    const int lightness = brightness + 127;//0...255

    int r = 0, g = 0, b = 0;

    for(int y = 0, w = img.width(), h = img.height(); y < h; y++){
        for(int x = 0; x < w; x++){
            QColor col (image.pixelColor(x, y));

            col.getRgb(&r,&g,&b);
            r += brightness;
            g += brightness;
            b += brightness;


            if(r>255)
                r=255;
            else if(r < 0)
                r = 0;
            if(g>255)
                g=255;
            else if(g < 0)
                g = 0;
            if(b>255)
                b=255;
            else if(b < 0)
                b = 0;


            image.setPixelColor(x, y, QColor(r,g,b));//.toRgb());
        }
    }
    return image;
}
//------------------------------------------------------------------------------------------------
QImage PrintImageHelper::makeContrastV2(const QImage &img, const int &contrast)
{
    if(contrast == 0)
        return img;
    //contrast 0-100%
    QImage image = img;// QImage(img.size(), format);
//    const int lightness = contrast + 127;//0...255


    for(int y = 0, w = img.width(), h = img.height(); y < h; y++){
        for(int x = 0; x < w; x++){
            QColor pixel (image.pixelColor(x, y));


            int l = pixel.saturation() ;
            l += contrast;
            if(l < 0)
                l = 0;
            else if(l > 255)
                 l = 255;

            pixel.setHsl(pixel.hue(), l, pixel.lightness(), pixel.alpha());

            image.setPixelColor(x, y, pixel);//.toRgb());
        }
    }
    return image;
}

//------------------------------------------------------------------------------------------------

QSizeF PrintImageHelper::getPageSize(const int &w, const int &h, const bool &isPortrait)
{
    return isPortrait ? QSizeF(w, h) : QSizeF(h, w);
}

//------------------------------------------------------------------------------------------------

PrintImageHelper::PrintSettCache PrintImageHelper::defaultPrintSett()
{
    PrintSettCache printSett;
    //Page
    printSett.widthMM           = 25;
    printSett.heightMM          = 30;    
    printSett.top               = 0;
    printSett.left              = 0;
    printSett.isPortrait        = false;
    printSett.resolutionDpi     = 300;
    printSett.genearateaqrcode = false;

    //Template
    //Image
    printSett.rotateDeg         = 0;
    printSett.colorIndx         = getSupportedImageColorConv().size() - 1;
    printSett.formatIndx        = getSupportedImageFormat().size() - 1;
    printSett.contrast          = 90;
    printSett.brightness        = 0;
    printSett.backgroundimagepath= "";
    printSett.defaultimagepath  = ":/katynko/label-print-def.png";
    printSett.usedefaultimage   = true;

    //Text
    printSett.dateMask          = getSupportedDateMask().first();
    printSett.textRotateDeg     = 0;
    printSett.fontPpt           = 19;
    printSett.textTopMargin     = 50;
    printSett.textLeftMargin    = 10;
    printSett.userData          = "$date\nNI:$ni";



    printSett.qrCorrLetter      = "M";
    printSett.enqrcustomposition= false;
    printSett.qrtopmarginpx     = 10;
    printSett.qrleftmarginpx    = 10;
    printSett.qrheightpx        = 200;
    printSett.qrwidthpx         = 200;

    return printSett;
}

//------------------------------------------------------------------------------------------------

PrintImageHelper::PrintSettCache PrintImageHelper::variantMap2printSett(const QVariantMap &m)
{
    const PrintSettCache defvals = defaultPrintSett();
    if(m.isEmpty())
        return defvals;

    PrintSettCache printSett;
//Page
    printSett.widthMM               = m.value("widthMM"             , defvals.widthMM           ).toInt() ;
    printSett.heightMM              = m.value("heightMM"            , defvals.heightMM          ).toInt() ;
    printSett.resolutionDpi         = m.value("resolutionDpi"       , defvals.resolutionDpi     ).toInt() ;
    printSett.isPortrait            = m.value("isPortrait"          , defvals.isPortrait        ).toBool();
    printSett.top                   = m.value("top"                 , defvals.top               ).toInt() ;
    printSett.left                  = m.value("left"                , defvals.left              ).toInt() ;
    printSett.genearateaqrcode      = m.value("genearateaqrcode"    , defvals.genearateaqrcode  ).toBool();

    //Template
    //Image
    printSett.rotateDeg             = m.value("rotateDeg"           , defvals.rotateDeg         ).toInt();
    printSett.formatIndx            = ValueValidator::getCorrectValueSigned(m.value("formatIndx", defvals.formatIndx).toInt(), PrintImageHelper::getSupportedImageFormat().size(), 0);
    printSett.colorIndx             = ValueValidator::getCorrectValueSigned(m.value("colorIndx" , defvals.colorIndx ).toInt(), PrintImageHelper::getSupportedImageFormat().size(), 0);
    printSett.contrast              = m.value("contrast"            , defvals.contrast          ).toInt();
    printSett.brightness            = m.value("brightness"          , defvals.brightness        ).toInt();
    printSett.backgroundimagepath   = m.value("backgroundimagepath" , defvals.backgroundimagepath).toString();
    printSett.defaultimagepath      = m.value("defaultimagepath"    , defvals.defaultimagepath).toString();
    printSett.usedefaultimage       = m.value("usedefaultimage"     , defvals.usedefaultimage   ).toBool();


    //Text
    const QStringList datemasks     = PrintImageHelper::getSupportedDateMask();
    printSett.dateMask              = datemasks.contains(m.value("dateMask", defvals.dateMask).toString()) ?
                m.value("dateMask", defvals.dateMask).toString() :
                datemasks.first();
    printSett.textRotateDeg         = m.value("textRotateDeg"       , defvals.textRotateDeg     ).toInt();
    printSett.fontPpt               = m.value("fontPpt"             , defvals.fontPpt           ).toInt();
    printSett.textTopMargin         = m.value("textTopMargin"       , defvals.textTopMargin     ).toInt();
    printSett.textLeftMargin        = m.value("textLeftMargin"      , defvals.textLeftMargin    ).toInt();
    printSett.userData              = m.value("userData"            , defvals.userData          ).toString();


//QR Code
    printSett.qrCorrLetter          = m.value("qrCorrLetter"        , defvals.qrCorrLetter      ).toString();
    printSett.enqrcustomposition    = m.value("enqrcustomposition"  , defvals.enqrcustomposition).toBool();// ui->groupBox_4->isChecked();
    printSett.qrtopmarginpx         = m.value("qrtopmarginpx"       , defvals.qrtopmarginpx     ).toInt();//ui->sbQrTopMargin->value();
    printSett.qrleftmarginpx        = m.value("qrleftmarginpx"      , defvals.qrleftmarginpx    ).toInt();//ui->sbQrLeftMargin->value();
    printSett.qrheightpx            = m.value("qrheightpx"          , defvals.qrheightpx        ).toInt();//ui->sbQrHeight->value();
    printSett.qrwidthpx             = m.value("qrwidthpx"           , defvals.qrwidthpx         ).toInt();//ui->sbQrHeight->value();//A QR code is a square


//    printSett.path2img = m.value("path2img", defvals.path2img).toString();
    return printSett;

}
//------------------------------------------------------------------------------------------------

QVariantMap PrintImageHelper::printSett2variantMap(const PrintImageHelper::PrintSettCache &printSett)
{
    QVariantMap m;
    //Page
    m.insert("widthMM"              , printSett.widthMM             );
    m.insert("heightMM"             , printSett.heightMM            );
    m.insert("top"                  , printSett.top                 );
    m.insert("left"                 , printSett.left                );
    m.insert("isPortrait"           , printSett.isPortrait          );
    m.insert("resolutionDpi"        , printSett.resolutionDpi       );
    m.insert("genearateaqrcode"     , printSett.genearateaqrcode    );

    //Template
    //Image
    m.insert("rotateDeg"            , printSett.rotateDeg           );
    m.insert("colorIndx"            , printSett.colorIndx           );
    m.insert("formatIndx"           , printSett.formatIndx          );
    m.insert("contrast"             , printSett.contrast            );
    m.insert("brightness"           , printSett.brightness          );
    m.insert("backgroundimagepath"  , printSett.backgroundimagepath );//  = ui->lePath2imgBackGround->text();
    m.insert("defaultimagepath"     , printSett.defaultimagepath    );//      = defaultimagepath;// "";
    m.insert("usedefaultimage"      , printSett.usedefaultimage     );//       = ui->cbxBackGround->isChecked();

    //Text
    m.insert("dateMask"             , printSett.dateMask            );
    m.insert("textRotateDeg"        , printSett.textRotateDeg       );
    m.insert("fontPpt"              , printSett.fontPpt             );
    m.insert("textTopMargin"        , printSett.textTopMargin       );
    m.insert("textLeftMargin"       , printSett.textLeftMargin      );
    m.insert("userData"             , printSett.userData            );


    m.insert("qrCorrLetter"         , printSett.qrCorrLetter        );
    m.insert("enqrcustomposition"   , printSett.enqrcustomposition  );//    = ui->groupBox_4->isChecked();
    m.insert("qrtopmarginpx"        , printSett.qrtopmarginpx       );//    printSett.qrtopmarginpx         = ui->sbQrTopMargin->value();
    m.insert("qrleftmarginpx"       , printSett.qrleftmarginpx      );//    printSett.qrleftmarginpx        = ui->sbQrLeftMargin->value();
    m.insert("qrheightpx"           , printSett.qrheightpx          );//    printSett.qrheightpx            = ui->sbQrHeight->value();
    m.insert("qrwidthpx"            , printSett.qrwidthpx           );//    printSett.qrwidthpx             = ui->sbQrHeight->value();//A QR code is a square




//    m.insert("path2img"     , printSett.path2img    );

    return m;
}

qreal PrintImageHelper::calculateDevicePixelRatio(const int &zoom, const int &minimum, const int &maximum)
{
    //minimum >= 0, maximum > 0
//1.0 = 100%
    //0.5 = 200%
    //1.5 = 50%

    const qreal half = (qreal)(minimum + maximum)/2.0;

//    if(zoom > 0)
//        return (qreal)((qreal)100.0/(qreal)zoom);
//    return (qreal)
    const qreal v = half/(qreal)zoom;
    return v;

}

//------------------------------------------------------------------------------------------------
