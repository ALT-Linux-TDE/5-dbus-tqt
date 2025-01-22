#################################################
#
#  (C) 2010-2011 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################


##### set project version ########################

include( TDEVersion )
cmake_minimum_required( VERSION ${TDE_CMAKE_MINIMUM_VERSION} )
tde_set_project_version( )


##### general package setup #####################

project( dbus-tqt-1 )


##### include essential cmake modules ###########

include( FindPkgConfig )
include( CheckCXXSourceCompiles )
include( CheckSymbolExists )

include( TDEMacros )
include( ConfigureChecks.cmake )


##### install paths setup #######################

tde_setup_install_path( EXEC_INSTALL_PREFIX   "${CMAKE_INSTALL_PREFIX}" )
tde_setup_install_path( LIB_INSTALL_DIR       "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}" )
tde_setup_install_path( INCLUDE_INSTALL_DIR   "${CMAKE_INSTALL_PREFIX}/include" )
tde_setup_install_path( PKGCONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/pkgconfig" )


##### install headers ###########################

install( FILES
    dbus-qt.h message.h connection.h server.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/dbus-1.0/dbus )


##### write pkgconfig file ######################

string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_EXEC_PREFIX ${EXEC_INSTALL_PREFIX} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_INCLUDE_DIR ${INCLUDE_INSTALL_DIR} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_LIB_DIR ${LIB_INSTALL_DIR} )

foreach( _lib ${PROJECT_NAME} ${DBUS_LIBRARIES} )
  set( PC_LIBRARIES "${PC_LIBRARIES} -l${_lib}" )
endforeach( )

configure_file( dbus-tqt.pc.cmake dbus-tqt.pc @ONLY )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/dbus-tqt.pc DESTINATION ${PKGCONFIG_INSTALL_DIR} )


##### build setup ###############################

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include tqt.h" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )

add_definitions(
  -DTQT_THREAD_SUPPORT
  ${TQT_CFLAGS_OTHER}
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${DBUS_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### dbus-tqt-1 (shared) #########################

tde_add_library( dbus-tqt-1 SHARED AUTOMOC
  SOURCES
    dbus-qthread.cpp message.cpp connection.cpp
    integrator.cpp server.cpp
  VERSION 0.0.0
  LINK ${TQT_LIBRARIES} ${DBUS_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
