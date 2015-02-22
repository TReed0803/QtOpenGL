#-------------------------------------------------------------------------------
# QtOpenGL Root Subdirs
#-------------------------------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

# Write the output binary path to a configuration file.
system(echo Creating config.pri.user... && echo BINARY_ROOT = \"$${OUT_PWD}\" > \"$${PWD}/config.pri.user\")

# Execute perl script to generate headers if perl is installed
system(where /q perl && cd $${PWD} && perl scripts/GenHeaders.pl)

SUBDIRS =     \
  qtbaseExt   \
  Karma       \
  OpenGL      \
  KarmaView
