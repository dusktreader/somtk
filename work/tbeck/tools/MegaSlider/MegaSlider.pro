CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(megasliderplugin)
TEMPLATE    = lib

HEADERS     = megasliderplugin.h
SOURCES     = megasliderplugin.cpp
LIBS        += -L.

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(megaslider.pri)
