
# See select_audio.txt for an implementation using more than 1 alternative

# Check for xevie
find_package(XEVIE)

IF(NOT XEVIE_FOUND)
  MESSAGE(FATAL_ERROR "XEvIE X Extension to capture Input Events was not found. See http://freedesktop.org/wiki/Software/XEvIE")
ENDIF(NOT XEVIE_FOUND)

INCLUDE_DIRECTORIES(${XEVIE_INCLUDE_DIRS})

SET(ANNOYME_LIBS_KEYINPUT ${ANNOYME_LIBS_KEYINPUT} ${XEVIE_LIBRARIES})
SET(ANNOYME_SRC_KEYINPUT ${ANNOYME_SRC_KEYINPUT} XevieInput.cpp)

