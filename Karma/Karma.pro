#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

TEMPLATE = lib
CONFIG += staticlib
TARGET = Karma
QT += widgets
include(../config.pri)

SOURCES += \
    kupdateevent.cpp \
    kcamera3d.cpp \
    ktransform3d.cpp \
    kinputmanager.cpp \
    kabstractobjparser.cpp \
    kfilereader.cpp \
    kbufferedfilereader.cpp \
    khalfedgemesh.cpp \
    khalfedgeobjparser.cpp \
    kabstractlexer.cpp \
    kabstractboundingvolume.cpp \
    kaabbboundingvolume.cpp \
    ksphereboundingvolume.cpp \
    kellipsoidboundingvolume.cpp \
    korientedboundingvolume.cpp

HEADERS += \
    kcolor.h \
    kcommon.h \
    kmacros.h \
    kupdateevent.h \
    kcamera3d.h \
    ktransform3d.h \
    kvertex.h \
    kmatrix4x4.h \
    kquaternion.h \
    krectf.h \
    kvector2d.h \
    kvector3d.h \
    kinputmanager.h \
    kpoint.h \
    kpointf.h \
    ksize.h \
    ksizef.h \
    kstring.h \
    ktouchpoint.h \
    kpinchgesture.h \
    kpangesture.h \
    kgesture.h \
    kabstractobjparser.h \
    kabstractreader.h \
    kfilereader.h \
    kbufferedfilereader.h \
    khalfedgemesh.h \
    khalfedgeobjparser.h \
    kabstractmesh.h \
    klinq.h \
    kparsetoken.h \
    kabstractlexer.h \
    kabstractwriter.h \
    kabstractboundingvolume.h \
    kaabbboundingvolume.h \
    ksphereboundingvolume.h \
    kellipsoidboundingvolume.h \
    korientedboundingvolume.h
