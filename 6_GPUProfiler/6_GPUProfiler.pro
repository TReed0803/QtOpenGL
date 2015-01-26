#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui

TARGET = 6_GPUProfiler
CONFIG   += console gl_debug
CONFIG   -= app_bundle
INCLUDEPATH += $$PWD/OpenGL

TEMPLATE = app

SOURCES += \
    main.cpp \
    window.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    profiler.cpp \
    frameresult.cpp \
    OpenGL/openglerror.cpp

HEADERS += \
    window.h \
    camera3d.h \
    input.h \
    transform3d.h \
    vertex.h \
    profiler.h \
    macros.h \
    frameresult.h \
    OpenGL/openglerror.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    OpenGL/openglcommon.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
