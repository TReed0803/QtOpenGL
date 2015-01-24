#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui

TARGET = 4_ErrorChecking
CONFIG   += console gl_debug
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    window.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    openglerror.cpp

HEADERS += \
    window.h \
    macros.h \
    fwdopengl.h \
    opengl.h \
    camera3d.h \
    input.h \
    transform3d.h \
    vertex.h \
    openglerror.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
