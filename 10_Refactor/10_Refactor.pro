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
INCLUDEPATH += $$PWD/Karma/Base
INCLUDEPATH += $$PWD/Karma/Events
INCLUDEPATH += $$PWD/Karma/Graphics
INCLUDEPATH += $$PWD/Karma/Math
INCLUDEPATH += $$PWD/Karma/Systems

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
    OpenGL/openglshaderprogram.cpp \
    OpenGL/openglprofilervisualizer.cpp \
    Karma/Events/kupdateevent.cpp \
    Karma/Graphics/kcamera3d.cpp \
    Karma/Graphics/kdebugdraw.cpp \
    Karma/Graphics/ktransform3d.cpp \
    Karma/Systems/kinputmanager.cpp \
    OpenGL/openglmarkerresult.cpp

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
    OpenGL/openglbuffer.h \
    OpenGL/openglshaderprogram.h \
    OpenGL/openglvertexarrayobject.h \
    OpenGL/openglprofilervisualizer.h \
    Karma/Base/kcolor.h \
    Karma/Base/kcommon.h \
    Karma/Base/kmacros.h \
    Karma/Events/kupdateevent.h \
    Karma/Graphics/kcamera3d.h \
    Karma/Graphics/kdebugdraw.h \
    Karma/Graphics/ktransform3d.h \
    Karma/Graphics/kvertex.h \
    Karma/Math/kmatrix4x4.h \
    Karma/Math/kquaternion.h \
    Karma/Math/krectf.h \
    Karma/Math/kvector2d.h \
    Karma/Math/kvector3d.h \
    Karma/Systems/kinputmanager.h \
    Karma/Math/kpoint.h \
    Karma/Math/kpointf.h \
    Karma/Math/ksize.h \
    Karma/Math/ksizef.h \
    Karma/Base/kstring.h

RESOURCES += \
    resources.qrc

CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
