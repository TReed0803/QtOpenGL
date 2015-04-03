#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

TEMPLATE  = app
CONFIG   += console
CONFIG   -= app_bundle
QT       += core gui widgets
TARGET    = KarmaView
include(../config.pri)

LIBS += $${KARMA_LIB}
LIBS += $${OPENGL_LIB}
LIBS += $${QTBASEEXT_LIB}

PRE_TARGETDEPS += $${KARMA_DEP}
PRE_TARGETDEPS += $${OPENGL_DEP}
PRE_TARGETDEPS += $${QTBASEEXT_DEP}

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    gbufferpass.cpp \
    compositionpass.cpp \
    samplescene.cpp \
    lightaccumulationpass.cpp \
    viewportpresentationpass.cpp \
    motionblurpass.cpp \
    preparepresentationpass.cpp \
    shadowedlightaccumulationpass.cpp \
    debuggbufferpass.cpp \
    environmentpass.cpp

HEADERS += \
    mainwidget.h \
    gbufferpass.h \
    compositionpass.h \
    renderpasses.h \
    samplescene.h \
    lightaccumulationpass.h \
    viewportpresentationpass.h \
    motionblurpass.h \
    preparepresentationpass.h \
    shadowedlightaccumulationpass.h \
    debuggbufferpass.h \
    environmentpass.h

RESOURCES += \
    ../resources.qrc \
    ../images.qrc
