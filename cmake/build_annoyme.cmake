
#
# Expects the following variables pre-set:
#   ANNOYME_SRC_SOUNDOUTPUT   - source files for the selected
#                               sound output types
#   ANNOYME_SRC_KEYINPUT      - source files for the selected
#                               key input types
#   ANNOYME_LIBS_COMMON       - Detected common libraries, required for
#                               the annoyme binary
#   ANNOYME_LIBS_SOUNDOUTPUT  - Detected sound output libraries, required for
#                               the annoyme binary
#   ANNOYME_LIBS_KEYINPUT     - Detected key input libraries, required for
#                               the annoyme binary

SET(ANNOYME_SRC
  annoyme-run.cpp
  Annoyme.cpp
  Event.cpp
  InputEventReaderFactory.cpp
  MixerOutput.cpp
  MixedOutput.cpp
  Sample.cpp
  SimpleWaveFileLoader.cpp
  SoundLoaderFactory.cpp
  SoundOutputFactory.cpp

  ResourceLoader.cpp
  util/FileUtil.cpp
  util/PathUtil.cpp
  config/AnnoymeConfiguration.cpp
  config/BasicConfiguration.cpp
  config/ConfigurationMap.cpp
  config/AggregateConfiguration.cpp
  config/SystemConfiguration.cpp
  config/SystemConfigurationLinux.cpp
  config/YAMLConfig.cpp

  HandlerSoundOutput.cpp
  SoundOutputAdapter.cpp
  Dispatcher.cpp
  ${ANNOYME_SRC_SOUNDOUTPUT}
  ${ANNOYME_SRC_KEYINPUT}
)

#
# Set Libs and Executables
#

OPTION(BUILD_DEBUGGING "Compile with debug mode." ON)
IF (BUILD_DEBUGGING)
       SET(CMAKE_BUILD_TYPE Debug)
       SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
ENDIF(BUILD_DEBUGGING)

SET(ANNOYME_EXECUTABLE "annoyme")

SET(CMAKE_CXX_FLAGS "-Wall")
ADD_DEFINITIONS(-D_REENTRANT)

ADD_EXECUTABLE(${ANNOYME_EXECUTABLE} ${ANNOYME_SRC})
# Add source dir to include path (for subdirs for example)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(annoyme ${ANNOYME_LIBS_COMMON})
TARGET_LINK_LIBRARIES(annoyme ${ANNOYME_LIBS_KEYINPUT})
TARGET_LINK_LIBRARIES(annoyme ${ANNOYME_LIBS_SOUNDOUTPUT})

