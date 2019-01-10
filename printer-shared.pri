INCLUDEPATH  += $$PWD\
                $$PWD/../../defines/defines\
                $$PWD/../../defines/define-types

HEADERS += \
    $$PWD/src/printer/printimagehelper.h \
    $$PWD/src/printer/qrcodegenerator.h \
    $$PWD/src/printer/qrcodegeneratortypes.h \
    $$PWD/src/printer/qrdevicedecoder.h \
    $$PWD/src/printer/qrgenconverter.h

SOURCES += \
    $$PWD/src/printer/printimagehelper.cpp \
    $$PWD/src/printer/qrcodegenerator.cpp \
    $$PWD/src/printer/qrdevicedecoder.cpp \
    $$PWD/src/printer/qrgenconverter.cpp


