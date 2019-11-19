#========================================================================================

project( lidar_livox )

#----------------------------------------------------------------------------------------

option( WITH_GUI ">>>>> With GUI <<<<<" FALSE )
if ( WITH_GUI )
    add_definitions( -DWITH_GUI )
    find_package( Qt5 COMPONENTS Gui REQUIRED )
endif()

#========================================================================================

cmake_minimum_required( VERSION 3.2 )

set( CMAKE_VERBOSE_MAKEFILE ON )

set( CMAKE_BUILD_TYPE "Debug" )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

#========================================================================================

set( QT5_MODULES Core Widgets PrintSupport )
find_package( Qt5 REQUIRED COMPONENTS ${QT5_MODULES} )

#========================================================================================

find_package( Boost 1.54 REQUIRED COMPONENTS system )
find_package( PkgConfig )
pkg_check_modules( APR apr-1 )

#========================================================================================

set( SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/livox_sdk" )

add_subdirectory( ${SDK_DIR}/sdk_core )

#========================================================================================

INCLUDE_DIRECTORIES(
    zcm_types/service/cpp_types
    )

INCLUDE_DIRECTORIES(
    config_reader
    lidar
    )

#========================================================================================

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vlibs" )
include( ${VLIBS_DIR}/vlog/vlog.cmake )
include( ${VLIBS_DIR}/vlog/vfilelog.cmake )
include( ${VLIBS_DIR}/vapplication/vapplication.cmake )
include( ${VLIBS_DIR}/vzcm/vzcm.cmake )
include( ${VLIBS_DIR}/vgit/vgit.cmake )
include( ${VLIBS_DIR}/vgio/keyfile/keyfile.cmake )

#========================================================================================

set(
    INC
    zcm_types/service/cpp_types/ZcmService.hpp
    ${V_HEADERS}
    )

#========================================================================================

set(
    INC
    config_reader/config.h
    lidar/lidar.h
    lidar/defs.h
    ${V_HEADERS}
    )

set(
    SRC
    config_reader/config.cpp
    lidar/lidar.cpp
    ${V_SOURCES}
    main.cpp
    cfg/lidar_livox.cfg
    )

#========================================================================================

add_executable( ${PROJECT_NAME} ${INC} ${SRC} ${qt5_use_modules} )

target_link_libraries(
    ${PROJECT_NAME}
    -llivox_sdk_static
    ${APR_LIBRARIES}
    Boost::boost
    Boost::system
    zcm
    ${V_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::PrintSupport )

unset ( QT_QMAKE_EXECUTABLE )

#========================================================================================
