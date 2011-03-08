
#
# Builds the test executable to print the x keymap changes
#

#
# Raw keycode printer
#

SET(MONITOR_PRINTER_SRC
  xquerykeymap/XKeyMapPrinter.cpp
  xquerykeymap/IllegalStateException.cpp
  xquerykeymap/XUtilException.cpp
  xquerykeymap/XKeyMapMonitor.cpp
  xquerykeymap/XKeyMapDifferenceFilter.cpp
  xquerykeymap/XKeyListenerPrinter.cpp
  xquerykeymap/XKeyMapListenerPrinter.cpp
  xquerykeymap/XKeyListenerBuilder.cpp
  xquerykeymap/XKeyMapSeparatorImpl.cpp
)

SET(XKEYMAPPRINTER_EXECUTABLE "xkeymapprinter")
ADD_EXECUTABLE(${XKEYMAPPRINTER_EXECUTABLE} ${MONITOR_PRINTER_SRC})

TARGET_LINK_LIBRARIES(${XKEYMAPPRINTER_EXECUTABLE} ${X11_LIBRARIES})
TARGET_LINK_LIBRARIES(${XKEYMAPPRINTER_EXECUTABLE} ${PTHREAD_LIBRARIES})

#
# keysymbol printer
#

SET(KEY_PRINTER_SRC
  xquerykeymap/XKeyPrinter.cpp
  xquerykeymap/IllegalStateException.cpp
  xquerykeymap/XUtilException.cpp
  xquerykeymap/XKeyMapMonitor.cpp
  xquerykeymap/XKeyMapDifferenceFilter.cpp
  xquerykeymap/XKeyListenerPrinter.cpp
  xquerykeymap/XKeyMapListenerPrinter.cpp
  xquerykeymap/XKeyListenerBuilder.cpp
  xquerykeymap/XKeyMapSeparatorImpl.cpp
)

SET(XKEYPRINTER_EXECUTABLE "xkeyprinter")
ADD_EXECUTABLE(${XKEYPRINTER_EXECUTABLE} ${KEY_PRINTER_SRC})

TARGET_LINK_LIBRARIES(${XKEYPRINTER_EXECUTABLE} ${X11_LIBRARIES})
TARGET_LINK_LIBRARIES(${XKEYPRINTER_EXECUTABLE} ${PTHREAD_LIBRARIES})
