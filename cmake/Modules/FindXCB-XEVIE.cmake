# - Try to find XCB-XEVIE
#
#
# Currently not used, only for experimenting!
#
# TODO rename variables to XKB-XEVIE
#
# Once done, this will define
#
#  XEVIE_FOUND - system has XEVIE
#  XEVIE_INCLUDE_DIRS - the XEVIE include directories
#  XEVIE_LIBRARIES - link these to use XEVIE
#
# See documentation on how to write CMake scripts at
# http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries

include(LibFindMacros)
include(CheckIncludeFile)

# Dependencies
libfind_package(XEVIE X11)

libfind_pkg_check_modules(XEVIE_PKGCONF xcb-xevie)

find_path(XEVIE_INCLUDE_DIR
  NAMES xcb/xevie.h
  PATHS ${XEVIE_PKGCONF_INCLUDE_DIRS}
)

find_library(XEVIE_LIBRARY
  NAMES xcb-xevie
  PATHS ${XEVIE_PKGCONF_LIBRARY_DIRS}
)

set(XEVIE_PROCESS_INCLUDES XEVIE_INCLUDE_DIR)
set(XEVIE_PROCESS_LIBS XEVIE_LIBRARY)

# MESSAGE(STATUS "XEV INC : " ${XEVIE_INCLUDE_DIR})
# MESSAGE(STATUS "XEV LIB : " ${XEVIE_LIBRARY})

libfind_process(XEVIE)

