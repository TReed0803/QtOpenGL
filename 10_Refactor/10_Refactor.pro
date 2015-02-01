#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 10_Refactor
CONFIG   += console gl_debug
CONFIG   -= app_bundle
INCLUDEPATH += $$PWD/OpenGL
INCLUDEPATH += $$PWD/Karma

TEMPLATE = app
unix:QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main.cpp \
    window.cpp \
    OpenGL/openglwindow.cpp \
    OpenGL/openglprofiler.cpp \
    OpenGL/openglframetimer.cpp \
    OpenGL/openglframeresults.cpp \
    OpenGL/openglerror.cpp \
    Karma/kinputmanager.cpp \
    Karma/kcamera3d.cpp \
    Karma/ktransform3d.cpp \
    OpenGL/openglshaderprogram.cpp \
    Karma/kupdateevent.cpp

HEADERS += \
    window.h \
    OpenGL/openglwindow.h \
    OpenGL/openglprofiler.h \
    OpenGL/openglframetimer.h \
    OpenGL/openglframeresults.h \
    OpenGL/openglmarkerresult.h \
    OpenGL/openglfunctions.h \
    OpenGL/openglcommon.h \
    OpenGL/openglerror.h \
    OpenGL/openglmarkerscoped.h \
    Karma/kmacros.h \
    Karma/kinputmanager.h \
    Karma/kcommon.h \
    Karma/kcamera3d.h \
    Karma/ktransform3d.h \
    Karma/kmatrix4x4.h \
    Karma/kvector3d.h \
    Karma/kvector2d.h \
    Karma/kvertex.h \
    Karma/kquaternion.h \
    OpenGL/openglbuffer.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    Karma/kupdateevent.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
