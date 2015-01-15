#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core

TARGET = KeyConverter
CONFIG   += console
CONFIG   -= app_bundle

# Add a Post-Link command that generates QKeyConverter class
CONFIG(debug, debug|release) {
QMAKE_POST_LINK += "$${OUT_PWD}/debug/KeyConverter.exe"
}
else {
QMAKE_POST_LINK += "$${OUT_PWD}/release/KeyConverter.exe"
}
QMAKE_POST_LINK += "../ProfileApplication/" "QKeyConverter"

TEMPLATE = app

SOURCES += \
    main.cpp

HEADERS += \
    keyobject.h

