#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 8_ES3Support
CONFIG   += console gl_debug
CONFIG   -= app_bundle
INCLUDEPATH += $$PWD/OpenGL

TEMPLATE = app
unix:QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main.cpp \
    window.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp \
    profiler.cpp \
    frameresult.cpp \
    debugdraw.cpp \
    OpenGL/shaderprogram.cpp \
    OpenGL/openglerror.cpp

HEADERS += \
    window.h \
    camera3d.h \
    input.h \
    transform3d.h \
    vertex.h \
    profiler.h \
    frameresult.h \
    debugdraw.h \
    debugvertex.h \
    macros.h \
    OpenGL/openglerror.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    OpenGL/openglcommon.h \
    OpenGL/openglmarkerscoped.h \
    OpenGL/openglfunctions.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
