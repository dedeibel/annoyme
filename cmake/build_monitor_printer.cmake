
#
# Builds the test executable to print the x keymap changes
#

SET(MONITOR_PRINTER_SRC
  xquerykeymap/XKeyMapMonitorPrinter.cpp
  xquerykeymap/IllegalStateException.cpp
  xquerykeymap/XUtilException.cpp
  xquerykeymap/XKeyMapMonitor.cpp
  xquerykeymap/XKeyMapListenerPrinter.cpp
)

OPTION(BUILD_DEBUGGING "Compile with debug mode." ON)
IF (BUILD_DEBUGGING)
       SET(CMAKE_BUILD_TYPE Debug)
       SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
ENDIF(BUILD_DEBUGGING)

SET(CMAKE_CXX_FLAGS "-Wall")
ADD_DEFINITIONS(-D_REENTRANT)

SET(XKEYMAPMONITORPRINTER_EXECUTABLE "xkeymapmonitorprinter")
ADD_EXECUTABLE(${XKEYMAPMONITORPRINTER_EXECUTABLE} ${MONITOR_PRINTER_SRC})

TARGET_LINK_LIBRARIES(${XKEYMAPMONITORPRINTER_EXECUTABLE} ${X11_LIBRARIES})
TARGET_LINK_LIBRARIES(${XKEYMAPMONITORPRINTER_EXECUTABLE} ${PTHREAD_LIBRARIES})