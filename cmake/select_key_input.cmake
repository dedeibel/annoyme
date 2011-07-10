
SET(INPUT_TYPES X11 XEVIE)
# INPUT_TYPES_AVAILABLE "declare" the variable :)
SET(INPUT_TYPES_FOUND FALSE)
SET(INPUT_TYPES_ENABLED FALSE)

FOREACH(TYPE ${INPUT_TYPES})
  # Check for sound output libraries, not fatal
  FIND_PACKAGE(${TYPE})
  
  IF(${TYPE}_FOUND)
    SET(INPUT_TYPES_FOUND TRUE)
    SET(INPUT_TYPES_AVAILABLE ${INPUT_TYPES_AVAILABLE} ${TYPE})
  ENDIF(${TYPE}_FOUND)

  OPTION(${TYPE}_ENABLED "Compile with ${TYPE} input method support enabled." ON)
ENDFOREACH(TYPE)

# Debugging purposes
#MESSAGE(STATUS "xeview found: ${XEVIE_FOUND}")

#
# X11
#
IF(X11_ENABLED AND X11_FOUND)
  SET(INPUT_TYPES_ENABLED TRUE)
  
  SET(XKEYMAP_INPUT_SRC
    xquerykeymap/IllegalStateException.cpp
    xquerykeymap/XUtilException.cpp
    xquerykeymap/XKeyMapMonitor.cpp
    xquerykeymap/XKeyMapDifferenceFilter.cpp
    xquerykeymap/XKeyListenerPrinter.cpp
    xquerykeymap/XKeyMapListenerPrinter.cpp
    xquerykeymap/XKeyListenerBuilder.cpp
    xquerykeymap/XKeyMapSeparatorImpl.cpp
    xquerykeymap/XKeyBufferingListener.cpp
    XKeyMapInput.cpp
  )
  
  INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
  SET(ANNOYME_LIBS_KEYINPUT ${ANNOYME_LIBS_KEYINPUT} ${X11_LIBRARIES})
  SET(ANNOYME_SRC_KEYINPUT ${ANNOYME_SRC_KEYINPUT} ${XKEYMAP_INPUT_SRC})
  SET(WITH_XKEYMAP TRUE)
ENDIF(X11_ENABLED AND X11_FOUND)

#
# XeVie
#
IF(XEVIE_ENABLED AND XEVIE_FOUND)
  SET(INPUT_TYPES_ENABLED TRUE)
  INCLUDE_DIRECTORIES(${XEVIE_INCLUDE_DIRS})
  SET(ANNOYME_LIBS_KEYINPUT ${ANNOYME_LIBS_KEYINPUT} ${XEVIE_LIBRARIES})
  SET(ANNOYME_SRC_KEYINPUT ${ANNOYME_SRC_KEYINPUT} XevieInput.cpp)
  SET(WITH_XEVIE TRUE)
ENDIF(XEVIE_ENABLED AND XEVIE_FOUND)

# There must be some of the above sound output libs available
IF(NOT INPUT_TYPES_FOUND)
  MESSAGE(FATAL_ERROR "No suitable \(supported\) key input method found.")
ENDIF(NOT INPUT_TYPES_FOUND)

# There must be some of the above sound output libs be enabled
IF(NOT INPUT_TYPES_ENABLED)
  MESSAGE(FATAL_ERROR "No key input method selected. Available are: ${INPUT_TYPES_AVAILABLE}")
ENDIF(NOT INPUT_TYPES_ENABLED)
