# - Try to find yaml-cpp
# Once done, this will define
#
#  YAMLCPP_FOUND        - system has yaml-cpp
#  YAMLCPP_INCLUDE_DIRS - the yaml-cpp include directories
#  YAML_LIBRARIES       - link these to use yaml-cpp
#
# See documentation on how to write CMake scripts at
# http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries

include(LibFindMacros)

libfind_pkg_check_modules(YAMLCPP_PKGCONF yaml-cpp)

find_path(YAMLCPP_INCLUDE_DIR
  NAMES yaml-cpp/yaml.h
  PATHS ${YAMLCPP_PKGCONF_INCLUDE_DIRS}
)

find_library(YAMLCPP_LIBRARY
  NAMES yaml-cpp
  PATHS ${YAMLCPP_PKGCONF_LIBRARY_DIRS}
)

set(YAMLCPP_PROCESS_INCLUDES YAMLCPP_INCLUDE_DIR)
set(YAMLCPP_PROCESS_LIBS YAMLCPP_LIBRARY)
libfind_process(YAMLCPP)

