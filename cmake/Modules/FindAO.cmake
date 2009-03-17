# - Try to find AO
# Once done, this will define
#
#  AO_FOUND - system has AO (GL and GLU)
#  AO_INCLUDE_DIRS - the AO include directories
#  AO_LIBRARIES - link these to use AO
#
# See documentation on how to write CMake scripts at
# http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries

include(LibFindMacros)

libfind_pkg_check_modules(AO_PKGCONF ao)

# lib: dl

find_path(AO_INCLUDE_DIR
  NAMES ao/ao.h
  PATHS ${AO_PKGCONF_INCLUDE_DIRS}
)

find_library(AO_LIBRARY
  NAMES ao
  PATHS ${AO_PKGCONF_LIBRARY_DIRS}
)

set(AO_PROCESS_INCLUDES AO_INCLUDE_DIR)
set(AO_PROCESS_LIBS AO_LIBRARY)
libfind_process(AO)

