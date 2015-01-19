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
win32 {
  CONFIG(debug, debug|release) {
  QMAKE_POST_LINK += "$${OUT_PWD}/debug/$${TARGET}.exe"
  }
  else {
  QMAKE_POST_LINK += "$${OUT_PWD}/release/$${TARGET}.exe"
  }
}
else {
  QMAKE_POST_LINK += "$${OUT_PWD}/$${TARGET}"
}
QMAKE_POST_LINK += "../ProfileApplication/" "QKeyConverter"

TEMPLATE = app

SOURCES += \
    main.cpp

HEADERS += \
    keyobject.h

