#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 9_ObjParser
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
    OpenGL/openglerror.cpp \
    OpenGL/halfedgemesh.cpp \
    OpenGL/filereader.cpp \
    OpenGL/halfedgeobjparser.cpp \
    OpenGL/abstractobjparser.cpp \
    OpenGL/abstractmesh.cpp \
    OpenGL/openglmesh.cpp

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
    OpenGL/openglfunctions.h \
    OpenGL/halfedgemesh.h \
    OpenGL/abstractreader.h \
    OpenGL/filereader.h \
    OpenGL/common.h \
    OpenGL/halfedgeobjparser.h \
    OpenGL/abstractobjparser.h \
    OpenGL/abstractmesh.h \
    OpenGL/openglmesh.h

RESOURCES += \
    resources.qrc \
    ../objects.qrc \
    ../objects.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
