#-------------------------------------------------------------------------------
# QtOpenGL Build Configuration
#-------------------------------------------------------------------------------

SOURCE_ROOT = $${PWD}
include($${SOURCE_ROOT}/config.pri.user)

SHARED_INCLUDES =         \
  $${PWD}                 \
  $${SOURCE_ROOT}/include \
  $${SOURCE_ROOT}/Karma   \
  $${SOURCE_ROOT}/OpenGL  \
  $${SOURCE_ROOT}/qtbaseExt/gui/opengl

android {
  DEFINES += "QT_OPENGL_ES_3"
}

win32:CONFIG(release, debug|release): OUT_SUBDIR = release/
win32:CONFIG(debug, debug|release): OUT_SUBDIR = debug/

win32 {
  LIB_EXT = .lib
} else {
  LIB_PREFIX = lib
  LIB_EXT = .a
}

defineReplace(defineLib) {
  return(-L$${BINARY_ROOT}/$$1/$${OUT_SUBDIR} -l$$1)
}

defineReplace(defineDep) {
  return ($${BINARY_ROOT}/$${1}/$${OUT_SUBDIR}$${LIB_PREFIX}$${1}$${LIB_EXT})
}

KARMA_LIB = $$defineLib(Karma)
OPENGL_LIB = $$defineLib(OpenGL)
QTBASEEXT_LIB = $$defineLib(qtbaseExt)

KARMA_DEP = $$defineDep(Karma)
OPENGL_DEP = $$defineDep(OpenGL)
QTBASEEXT_DEP = $$defineDep(qtbaseExt)

INCLUDEPATH += $${SHARED_INCLUDES}

RESOURCES += \
    $$PWD/resources.qrc

HEADERS +=

SOURCES +=
