#========================================================================================

TARGET = lidar_livox

#========================================================================================

DEFINES += GUI

#========================================================================================

QT += core
QT -= gui
QT += network

#========================================================================================

TEMPLATE += app

CONFIG *= link_pkgconfig
CONFIG += c++11

#========================================================================================

DEFINES += QT_DEPRECATED_WARNINGS

MAIN_DIR = $$PWD

#========================================================================================

SDK_DIR = $$PWD/livox_sdk
include( $$SDK_DIR/sdk_core/sdk_core.pri )

VLIBS_DIR = $$PWD/vlibs2
include( $$PWD/vlibs.pri )

#========================================================================================

SOURCES += main.cpp

#========================================================================================

include( $$PWD/config_reader/config_reader.pri )
include( $$PWD/lidar/lidar.pri )
include( $$PWD/defs/defs.pri )

contains( DEFINES, GUI ): include( $$PWD/plot/plot.pri )

#========================================================================================

OTHER_FILES += cfg/*
OTHER_FILES += scripts/*

#========================================================================================

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /tmp/$${TARGET}/bin
!isEmpty( target.path ): INSTALLS += target

#========================================================================================
