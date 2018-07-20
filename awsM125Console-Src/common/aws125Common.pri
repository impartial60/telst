
INCLUDEPATH += $$PWD

SRC=$$PWD/../aws125CommonSrc

SOURCES += \
    $$SRC/encoder_control.cpp \
    $$SRC/crc16Utils.cpp \
    $$SRC/diagnParserUtils.cpp \
    $$SRC/sharedMemoryOptions.cpp \
    $$SRC/multiLanguageSupport.cpp


HEADERS += \
    $$PWD/usm_mdo_common.h \
    $$PWD/usm.h \
    $$PWD/mdo.h \
    $$PWD/sharedMem.h \
    $$PWD/snrServiceInfo.h \
    $$PWD/ironBtnsG.h \
    $$PWD/targetPlot.h \
    $$PWD/localstructures.h \
    $$PWD/commonEnum.h \
    $$PWD/externCommon.h
