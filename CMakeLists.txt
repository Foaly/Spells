cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "Spells")
project(${PROJECT_NAME})

# include helpers
include(${CMAKE_CURRENT_SOURCE_DIR}/helpers.cmake)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

message(STATUS "Configuring in ${CMAKE_BUILD_TYPE} mode.")

# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# define sources
set(EXECUTABLE_NAME "${PROJECT_NAME}")
include_directories("src/*")
set(SOURCE_FILES src/main.cpp
                 src/Spells.hpp
                 src/Spells.cpp
                 src/SpellGenerator.hpp
                 src/SpellGenerator.cpp
                 src/Util.hpp
                 src/Util.cpp
                 src/VectorEmitter.hpp
                 src/VectorEmitter.cpp
                 src/FallingPointEmitter.hpp
                 src/FallingPointEmitter.cpp
                 src/PathResolver.hpp
                 src/BezierCurve.hpp
                 src/BezierCurve.cpp
                 src/nanosvg/nanosvg.h
                 src/SettingsParser/SettingsParser.hpp
                 src/SettingsParser/SettingsParser.cpp
                 src/Level.hpp
                 src/Level.cpp
                 src/LoadHelpers.hpp
                 src/LoadHelpers.cpp
                 src/Clock.hpp
                 src/Clock.cpp
                 src/ArcSegment.hpp
                 src/ArcSegment.cpp
                 src/Ellipse.hpp
                 src/Ellipse.cpp
                 )

# when we are on MacOS create a bundle and
# put the rescources in it
if(APPLE)
    set(SOURCE_FILES ${SOURCE_FILES}
                     src/osx/PathResolver.mm)

    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES})
    target_link_libraries( ${EXECUTABLE_NAME} "-framework Foundation" )

    # copy resource files into bundle
    copy_resource_folder_to_bundle(${EXECUTABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/rundirectory/data data)
  	set_target_properties(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/osx/Info.plist.in)
else()
    # on windows there are no bundles
    set(SOURCE_FILES ${SOURCE_FILES}
                     src/PathResolver.cpp)

    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif()


# Add our CMake scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})


# Detect and add any version 2.X of SFML
# See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
else()
    message(FATAL_ERROR "Unable to find SFML!")
endif()


# Detect and add Thor
find_package(Thor 2 REQUIRED)
if(THOR_FOUND)
    include_directories(${THOR_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${THOR_LIBRARY})
else()
    message(FATAL_ERROR "Unable to find Thor!")
endif()
