#========================================================================================

TARGET = lidar_livox

#========================================================================================

DEFINES -= WITH_GUI

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

ZCM_Dir = /usr/local
INCLUDEPATH += $$ZCM_Dir/include/
LIBS += -L$$ZCM_Dir/lib -lzcm

ZCM_DIR = $$PWD/zcm_types
include( $$PWD/zcm_types.pri )
LIBS += -L$$ZCM_DIR -lzcmtypes

#========================================================================================

SOURCES += main.cpp

#========================================================================================

include( $$PWD/config_reader/config_reader.pri )
include( $$PWD/lidar/lidar.pri )
include( $$PWD/defs/defs.pri )
include( $$PWD/publish/publish.pri )

contains( DEFINES, WITH_GUI ): include( $$PWD/plot/plot.pri )

#========================================================================================

OTHER_FILES += cfg/*
OTHER_FILES += scripts/*

#========================================================================================
