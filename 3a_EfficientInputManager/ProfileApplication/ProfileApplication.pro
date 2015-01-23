#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core

TARGET = ProfileApplication
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    qkeyconverter.cpp \
    qkeyconvertercustom.cpp \
    qkeyrandom.cpp \
    Instances/KeyConverted.cpp \
    Instances/KeyConvertedCustom.cpp \
    Instances/KeyLimitedRange.cpp \
    Instances/KeyLimitedRangeSimple.cpp \
    Instances/QKeyList.cpp \
    Instances/QKeyMap.cpp \
    Instances/QKeyVector.cpp \
    Instances/StdKeyList.cpp \
    Instances/StdKeyMap.cpp \
    Instances/StdKeyUnorderedMap.cpp \
    Instances/StdKeyVector.cpp \
    Instances/QInput.cpp

HEADERS += \
    keyobject.h \
    qkeyconverter.h \
    tests.h \
    tests.inc \
    Instances/KeyConverted.h \
    Instances/KeyConvertedCustom.h \
    Instances/KeyLimitedRange.h \
    Instances/KeyLimitedRangeSimple.h \
    Instances/StdKeyList.h \
    Instances/StdKeyMap.h \
    Instances/StdKeyUnorderedMap.h \
    Instances/StdKeyVector.h \
    Instances/QKeyList.h \
    Instances/QKeyMap.h \
    Instances/QKeyVector.h \
    Instances/QInput.h \
    keyconvertercustom.h \
    keyrandom.h
