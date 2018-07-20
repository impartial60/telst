QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = awsM125Console
CONFIG += console
CONFIG -= app_bundle

DESTDIR = $$_PRO_FILE_PWD_/../

TEMPLATE = app

SOURCES += \
    threadsAll.cpp \
    threadCnvEth485.cpp \
    threadDiagnosticMonitor.cpp \
    threadMDO1.cpp \
    keyboard_keys.cpp \
    servoDriveExecuter.cpp \
    rs.cpp \
    threadSYS.cpp \
    threadLauncherOfficer.cpp \
    threadCommanderInfoTable.cpp \
    asterix126Coder.cpp \
    threadVKP.cpp \
    threadCommander.cpp \
    threadGuidanceOfficer.cpp \
#    ../../aws125CommonSrc/sharedMemoryOptions.cpp \
#    ../../aws125CommonSrc/tcpSocketOptions.cpp \
#    ../../aws125CommonSrc/commonUtils.cpp \
    threadSimulator.cpp \
    threadP18.cpp \ #\
    srcKram/clockrealtime.cpp \
    srcKram/device125.cpp \
    srcKram/Drive125.cpp \
    common/aws125CommonSrc/commonUtils.cpp \
 #   common/aws125CommonSrc/crc16Utils.cpp \
#    common/aws125CommonSrc/diagnParserUtils.cpp \
 #   common/aws125CommonSrc/drawScales.cpp \
#    common/aws125CommonSrc/multiLanguageSupport.cpp \
    common/aws125CommonSrc/sharedMemoryOptions.cpp \
    common/aws125CommonSrc/tcpSocketOptions.cpp \
    main_console.cpp \
    srcKram/main.cpp



# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    extern.h \
    threadstatus.h \
    keyboard_keys.h \
    mainConsoleSettings.h \
    srcKram/clockrealtime.h \
    srcKram/device125.h \
    srcKram/Drive125.h

INCLUDEPATH += Common
#                /opt/menlinux/INCLUDE/COM\
#               /opt/menlinux/INCLUDE/NATIVE\


//unix:!macx: LIBS += -L/usr/local/lib/ -lmdis_api
//unix:!macx: LIBS += -L/usr/local/lib/ -lusr_oss

DISTFILES += \
    ВоронинуАА


