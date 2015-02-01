#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 10_Refactor
CONFIG   += console gl_debug
CONFIG   -= app_bundle
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/includes
INCLUDEPATH += $$PWD/includes/OpenGL
INCLUDEPATH += $$PWD/includes/Karma
INCLUDEPATH += $$PWD/includes/Karma/Base
INCLUDEPATH += $$PWD/includes/Karma/Math
INCLUDEPATH += $$PWD/includes/Karma/Events
INCLUDEPATH += $$PWD/includes/Karma/Graphics
INCLUDEPATH += $$PWD/includes/Karma/Systems

TEMPLATE = app
unix:QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    src/main.cpp \
    src/window.cpp \
    src/OpenGL/openglwindow.cpp \
    src/OpenGL/openglprofiler.cpp \
    src/OpenGL/openglframetimer.cpp \
    src/OpenGL/openglframeresults.cpp \
    src/OpenGL/openglerror.cpp \
    src/OpenGL/openglshaderprogram.cpp \
    src/OpenGL/openglprofilervisualizer.cpp \
    src/Karma/Events/kupdateevent.cpp \
    src/Karma/Graphics/kcamera3d.cpp \
    src/Karma/Graphics/kdebugdraw.cpp \
    src/Karma/Graphics/ktransform3d.cpp \
    src/Karma/Systems/kinputmanager.cpp \
    src/OpenGL/openglmarkerresult.cpp

HEADERS += \
    src/window.h \
    src/OpenGL/openglwindow.h \
    src/OpenGL/openglprofiler.h \
    src/OpenGL/openglframetimer.h \
    src/OpenGL/openglframeresults.h \
    src/OpenGL/openglmarkerresult.h \
    src/OpenGL/openglfunctions.h \
    src/OpenGL/openglcommon.h \
    src/OpenGL/openglerror.h \
    src/OpenGL/openglmarkerscoped.h \
    src/OpenGL/openglbuffer.h \
    src/OpenGL/openglshaderprogram.h \
    src/OpenGL/openglvertexarrayobject.h \
    src/OpenGL/openglprofilervisualizer.h \
    src/Karma/Base/kcolor.h \
    src/Karma/Base/kcommon.h \
    src/Karma/Base/kmacros.h \
    src/Karma/Events/kupdateevent.h \
    src/Karma/Graphics/kcamera3d.h \
    src/Karma/Graphics/kdebugdraw.h \
    src/Karma/Graphics/ktransform3d.h \
    src/Karma/Graphics/kvertex.h \
    src/Karma/Math/kmatrix4x4.h \
    src/Karma/Math/kquaternion.h \
    src/Karma/Math/krectf.h \
    src/Karma/Math/kvector2d.h \
    src/Karma/Math/kvector3d.h \
    src/Karma/Systems/kinputmanager.h \
    src/Karma/Math/kpoint.h \
    src/Karma/Math/kpointf.h \
    src/Karma/Math/ksize.h \
    src/Karma/Math/ksizef.h \
    src/Karma/Base/kstring.h

RESOURCES += \
    resources.qrc

CONFIG += mobility
CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
