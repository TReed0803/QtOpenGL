#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = KeyConverter \
          ProfileApplication

ProfileApplication.depends = KeyConverter
