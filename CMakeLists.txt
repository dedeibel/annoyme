
cmake_minimum_required(VERSION 2.6)
project(ANNOYME)

#
# Setup and includes
#

# Add the module include path for custom lib finders
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

INCLUDE(CheckIncludeFiles)

#
# Package Checks
#

# Check for pthread
CHECK_INCLUDE_FILES(pthread.h HAVE_PTHREAD_H)
IF(NOT HAVE_PTHREAD_H)
  MESSAGE(FATAL_ERROR "pthread.h not found.")
ENDIF(NOT HAVE_PTHREAD_H)

# Check for xevie
find_package(XEVIE)
MESSAGE(STATUS "XEV: ${XEVIE_FOUND}")
MESSAGE(STATUS "XEVDIR: ${XEVIE_INCLUDE_DIRS}")
IF(NOT XEVIE_FOUND)
  MESSAGE(FATAL_ERROR "XEvIE X Extension to capture Input Events was not found. See http://freedesktop.org/wiki/Software/XEvIE")
ENDIF(NOT XEVIE_FOUND)
INCLUDE_DIRECTORIES(${XEVIE_INCLUDE_DIRS})
SET(ANNOYME_LIBS ${ANNOYME_LIBS} ${XEVIE_LIBRARIES})
SET(ANNOYME_SRC_REQUIRED ${ANNOYME_SRC_REQUIRED} XevieInput.cpp)


# Check for sound output libraries, not fatal
find_package(ALSA)
find_package(AO)

# There must be some of the above sound output libs available
IF(    NOT ALSA_FOUND
   AND NOT AO_FOUND
  )
  MESSAGE(FATAL_ERROR "No suitable \(supported\) sound output found.")
ENDIF()

#
# Build customization according to package checks
#

SET(WITH_ALSA 1)
SET(WITH_AO 1)

IF(WITH_ALSA)
  SET(ANNOYME_SRC_SOUNDOUTPUT ${ANNOYME_SRC_SOUNDOUTPUT} AlsaOutput.cpp)
  SET(ANNOYME_LIBS ${ANNOYME_LIBS} ${ALSA_LIBRARIES})
  INCLUDE_DIRECTORIES(${ALSA_INCLUDE_DIRS})
ENDIF(WITH_ALSA)

IF(WITH_AO)
  SET(ANNOYME_SRC_SOUNDOUTPUT ${ANNOYME_SRC_SOUNDOUTPUT} AOutput.cpp)
  SET(ANNOYME_LIBS ${ANNOYME_LIBS} ${AO_LIBRARIES})
  INCLUDE_DIRECTORIES(${AO_INCLUDE_DIRS})
ENDIF(WITH_AO)

SET(ANNOYME_SRC
  annoyme.cpp
  Annoyme.cpp
  Event.cpp
  FileConfiguration.cpp
  InputEventReaderFactory.cpp
  MixerOutput.cpp
  Sample.cpp
  SimpleWaveFileLoader.cpp
  SoundLoaderFactory.cpp
  SoundOutputFactory.cpp
  StaticConfiguration.cpp
  ${ANNOYME_SRC_REQUIRED}
  ${ANNOYME_SRC_SOUNDOUTPUT}
)

#
# Set Libs and Executables
#

ADD_EXECUTABLE(annoyme ${ANNOYME_SRC})
TARGET_LINK_LIBRARIES(annoyme ${ANNOYME_LIBS})

#
# Set config variables
#

# Where to finde the sample files, TODO add install path and relative path here
SET(ANNOYME_SAMPLE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/include/)
