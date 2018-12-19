#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T19:00:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = gpscon
sailfish: CONFIG += sailfishapp
sailfish: QT += location
CONFIG   += console
CONFIG   -= app_bundle
!maemo5:!sailfish: CONFIG += mobility
maemo5: CONFIG += mobility12
maemo5: QMAKE_RPATHDIR += /opt/qtm12/lib
!sailfish: MOBILITY += location
TEMPLATE = app


SOURCES += src/main.cpp \
    src/gpspos.cpp \
    src/mainclass.cpp \
    src/qargparser.cpp \
    src/consolereader.cpp

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

HEADERS += \
    src/gpspos.h \
    src/mainclass.h \
    src/qargparser.h \
    src/consolereader.h

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/gpscon/bin
    INSTALLS += target
}

maemo5 {
    target.path = /opt/gpscon/bin
    INSTALLS += target
}
sailfish {
    DEFINES += Q_OS_SAILFISH
}
