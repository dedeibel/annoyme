
IF(BUILD_TESTING)

# Check for cppunit
find_package(CppUnit)

IF(NOT CPPUNIT_FOUND)
  MESSAGE(FATAL_ERROR "cppunit was not found.")
ENDIF(NOT CPPUNIT_FOUND)

FILE(GLOB ConfigSRC
    ${CMAKE_CURRENT_SOURCE_DIR}/config/BasicConfiguration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/ConfigurationMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/AnnoymeConfiguration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/SystemConfiguration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/AggregateConfiguration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/YAMLConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/SystemConfigurationLinux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/FileUtil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/PathUtil.cpp
)
SET(UnitTestDep_SRCS ${ConfigSRC})

# Search for all files ending with Test.cpp
FILE(GLOB_RECURSE UnitTests_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*Test.cpp" )
# Create the test runner and linking including all the unit tests
ADD_EXECUTABLE(UnitTester test_runner.cpp ${UnitTests_SRCS} ${UnitTestDep_SRCS})
# Link cppunit to the test runner
TARGET_LINK_LIBRARIES(UnitTester cppunit)
# Register each test with cmake
FOREACH(test ${UnitTests_SRCS})
        GET_FILENAME_COMPONENT(TestName ${test} NAME_WE)
        ADD_TEST(${TestName} ${EXECUTABLE_OUTPUT_PATH}/UnitTester ${TestName})
ENDFOREACH(test)

TARGET_LINK_LIBRARIES(UnitTester ${ANNOYME_LIBS_COMMON})

# Build the test runner after the main executable
# but this is not usefull since it might break the test suite if
# there is a problem with creating the executable. They do not share
# the object files anyway. See cmake wiki: http://goo.gl/qK1C
#ADD_DEPENDENCIES(UnitTester ${ANNOYME_EXECUTABLE})

ENDIF(BUILD_TESTING)
