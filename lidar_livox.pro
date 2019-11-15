#========================================================================================

TARGET = lidar_livox

#========================================================================================

DEFINES += WITH_GUI="true"

#========================================================================================

QT += core
QT -= gui declarative

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#========================================================================================

TEMPLATE += app

CONFIG *= link_pkgconfig
CONFIG += c++11

#========================================================================================

DEFINES += QT_DEPRECATED_WARNINGS

#========================================================================================

ZCM_Dir = /usr/local
INCLUDEPATH += $$ZCM_Dir/include/
LIBS += -L$$ZCM_Dir/lib -lzcm

ZCM_TYPES_Dir = $$PWD/zcm_types
INCLUDEPATH += $$ZCM_TYPES_Dir/service/cpp_types/
LIBS += -L$$ZCM_TYPES_Dir -lzcmtypes

#========================================================================================s

SOURCES += main.cpp 

MAIN_DIR = $$PWD

#========================================================================================

VLIBS_DIR = $$PWD/vlibs
include( $$PWD/vlibs.pri )

config_reader_dir = $$PWD/config_reader
INCLUDEPATH += $$config_reader_dir
include( $$config_reader_dir/config_reader.pri )

#========================================================================================

DISTFILES += cfg/lidar_livox.cfg

#========================================================================================

