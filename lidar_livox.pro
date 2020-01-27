#========================================================================================

TARGET = lidar_livox

#========================================================================================

DEFINES += WITH_GUI

#========================================================================================

QT += core
QT -= gui
QT += widgets
QT += network

#========================================================================================

TEMPLATE += app

CONFIG *= link_pkgconfig
CONFIG += c++11

#========================================================================================

DEFINES += QT_DEPRECATED_WARNINGS

#========================================================================================

SDK_DIR = $$PWD/livox_sdk
include( $$SDK_DIR/sdk_core/sdk_core.pri )
#include( $$PWD/sdk.pri )

VLIBS_DIR = $$PWD/vlibs2
include( $$PWD/vlibs.pri )

#========================================================================================

SOURCES += main.cpp

#========================================================================================

#include( $$PWD/config_reader/config_reader.pri )
include( $$PWD/lidar/lidar.pri )
include( $$PWD/plot/plot.pri )

#========================================================================================

OTHER_FILES += cfg/lidar_livox.cfg

#========================================================================================

