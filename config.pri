#-------------------------------------------------------------------------------
# QtOpenGL Build Configuration
#-------------------------------------------------------------------------------

SOURCE_ROOT = $${PWD}
include($${SOURCE_ROOT}/config.pri.user)

SHARED_INCLUDES =         \
  $${PWD}                 \
  $${SOURCE_ROOT}/Karma   \
  $${SOURCE_ROOT}/OpenGL

win32:CONFIG(release, debug|release): OUT_SUBDIR = release/
win32:CONFIG(debug, debug|release): OUT_SUBDIR = debug/

win32 {
  LIB_EXT = .lib
} else {
  LIB_EXT = .a
}

defineReplace(defineLib) {
  return(-L$${BINARY_ROOT}/$$1/$${OUT_SUBDIR} -l$$1)
}

defineReplace(defineDep) {
  return ($${BINARY_ROOT}/$${1}/$${OUT_SUBDIR}$${1}$${LIB_EXT})
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
