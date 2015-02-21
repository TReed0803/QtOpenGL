#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = 21_GBufferConstruction
CONFIG   += console gl_debug
CONFIG   -= app_bundle
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/includes
INCLUDEPATH += $$PWD/includes/OpenGL
INCLUDEPATH += $$PWD/includes/Karma
INCLUDEPATH += $$PWD/includes/qtbase/gui/opengl
DEFINES += "_ITERATOR_DEBUG_LEVEL=0"
android {
  DEFINES += "QT_OPENGL_ES_3"
}

TEMPLATE = app
unix:QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    src/main.cpp \
    src/OpenGL/openglprofiler.cpp \
    src/OpenGL/openglframetimer.cpp \
    src/OpenGL/openglframeresults.cpp \
    src/OpenGL/openglerror.cpp \
    src/OpenGL/openglshaderprogram.cpp \
    src/OpenGL/openglprofilervisualizer.cpp \
    src/OpenGL/openglmarkerresult.cpp \
    src/Karma/kupdateevent.cpp \
    src/Karma/kcamera3d.cpp \
    src/Karma/kdebugdraw.cpp \
    src/Karma/ktransform3d.cpp \
    src/Karma/kinputmanager.cpp \
    src/OpenGL/openglwidget.cpp \
    src/mainwidget.cpp \
    src/Karma/kabstractobjparser.cpp \
    src/Karma/kfilereader.cpp \
    src/Karma/kbufferedfilereader.cpp \
    src/Karma/khalfedgemesh.cpp \
    src/Karma/khalfedgeobjparser.cpp \
    src/OpenGL/openglmesh.cpp \
    src/qtbase/gui/opengl/qopenglfunctions_es3_0.cpp \
    src/OpenGL/opengluniformbufferobject.cpp \
    src/Karma/kabstractlexer.cpp \
    src/OpenGL/openglslparser.cpp \
    src/OpenGL/openglinstancegroup.cpp \
    src/OpenGL/openglframebufferobject.cpp \
    src/OpenGL/openglrenderbufferobject.cpp \
    src/OpenGL/opengltexture.cpp \
    src/OpenGL/openglmaterial.cpp \
    src/OpenGL/openglinstance.cpp

HEADERS += \
    includes/OpenGL/openglprofiler.h \
    includes/OpenGL/openglframetimer.h \
    includes/OpenGL/openglframeresults.h \
    includes/OpenGL/openglmarkerresult.h \
    includes/OpenGL/openglfunctions.h \
    includes/OpenGL/openglcommon.h \
    includes/OpenGL/openglerror.h \
    includes/OpenGL/openglmarkerscoped.h \
    includes/OpenGL/openglbuffer.h \
    includes/OpenGL/openglshaderprogram.h \
    includes/OpenGL/openglvertexarrayobject.h \
    includes/OpenGL/openglprofilervisualizer.h \
    includes/Karma/kcolor.h \
    includes/Karma/kcommon.h \
    includes/Karma/kmacros.h \
    includes/Karma/kupdateevent.h \
    includes/Karma/kcamera3d.h \
    includes/Karma/kdebugdraw.h \
    includes/Karma/ktransform3d.h \
    includes/Karma/kvertex.h \
    includes/Karma/kmatrix4x4.h \
    includes/Karma/kquaternion.h \
    includes/Karma/krectf.h \
    includes/Karma/kvector2d.h \
    includes/Karma/kvector3d.h \
    includes/Karma/kinputmanager.h \
    includes/Karma/kpoint.h \
    includes/Karma/kpointf.h \
    includes/Karma/ksize.h \
    includes/Karma/ksizef.h \
    includes/Karma/kstring.h \
    includes/Karma/ktouchpoint.h \
    includes/Karma/kpinchgesture.h \
    includes/Karma/kpangesture.h \
    includes/Karma/kgesture.h \
    includes/OpenGL/openglwidget.h \
    includes/mainwidget.h \
    includes/Karma/kabstractobjparser.h \
    includes/Karma/kabstractreader.h \
    includes/Karma/kfilereader.h \
    includes/Karma/kbufferedfilereader.h \
    includes/Karma/khalfedgemesh.h \
    includes/Karma/khalfedgeobjparser.h \
    includes/Karma/kabstractmesh.h \
    includes/OpenGL/openglmesh.h \
    includes/Karma/kchunkvector.h \
    includes/Karma/klinq.h \
    includes/qtbase/gui/opengl/qopenglfunctions_es3_0.h \
    includes/OpenGL/openglfunctions_es3_0.h \
    includes/OpenGL/openglfunctions_3_3_core.h \
    includes/OpenGL/opengluniformbufferobject.h \
    includes/Karma/kparsetoken.h \
    includes/Karma/kabstractlexer.h \
    includes/OpenGL/openglslparser.h \
    includes/OpenGL/openglinstancegroup.h \
    includes/OpenGL/openglframebufferobject.h \
    includes/OpenGL/opengltexture.h \
    includes/OpenGL/openglrenderbufferobject.h \
    includes/OpenGL/openglmaterial.h \
    includes/OpenGL/openglinstance.h

RESOURCES += \
    resources.qrc \
    ../objects.qrc

CONFIG += mobility
CONFIG(debug,debug|release) {
  CONFIG(gl_debug) {
      DEFINES += GL_DEBUG
  }
}
