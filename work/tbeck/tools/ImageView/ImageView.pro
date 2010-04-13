CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(imageviewplugin)
TEMPLATE    = lib

HEADERS     = imageviewplugin.h
SOURCES     = imageviewplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L.

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

QMAKE_CLEAN += libimageviewplugin.so Makefile Makefile.Debug Makefile.Release

include(imageview.pri)
