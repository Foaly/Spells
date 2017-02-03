cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME "Spells")
project(${PROJECT_NAME})


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
set(SOURCE_FILES main.cpp
                 Spells.hpp
                 Spells.cpp
                 SpellGenerator.hpp
                 SpellGenerator.cpp
                 Util.hpp
                 Util.cpp
                 VectorEmitter.hpp
                 VectorEmitter.cpp
                 FallingPointEmitter.hpp
                 FallingPointEmitter.cpp
                 PathResolver.hpp
                 )

# when we are on MacOS create a bundle and
# put the rescources in it
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SOURCE_FILES ${SOURCE_FILES}
                     PathResolver.mm)

    file(GLOB_RECURSE RESOURCE_FILES "rundirectory/data/*")

    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCE_FILES})

    set_source_files_properties(${RESOURCE_FILES}
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
    # on windows there are no bundles
    set(SOURCE_FILES ${SOURCE_FILES}
                     PathResolver.cpp)

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
endif()


# Detect and add Thor
find_package(Thor 2 REQUIRED)
if(THOR_FOUND)
    include_directories(${THOR_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${THOR_LIBRARY})
endif()

# Install target
#install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
