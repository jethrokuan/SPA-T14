cmake_minimum_required (VERSION 3.9.2)
project (autotester)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
message (STATUS "${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}")
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are forbidden.")
endif()

ADD_DEFINITIONS(
  -std=c++1z # Or -std=c++0x
  # Other flags
  )

if (WIN32)
  SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
  set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/win")
elseif (APPLE)
  set(Qt5Core_DIR /usr/local/opt/qt/lib/cmake/Qt5Core)
  set(Qt5Widgets_DIR /usr/local/opt/qt/lib/cmake/Qt5Widgets)
  set(Qt5Gui_DIR /usr/local/opt/qt/lib/cmake/Qt5Gui)
  set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/apple")
elseif (UNIX)
  set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/unix")
endif()


list(APPEND CMAKE_PREFIX_PATH "${AUTOTESTER_ROOT}")
find_package(Autotester REQUIRED)
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib")#include catch.hpp

add_subdirectory(src/spa)
add_subdirectory(src/autotester)
# add_subdirectory(src/autotester_gui)
add_subdirectory(src/unit_testing)
add_subdirectory(src/integration_testing)

enable_testing()
add_test(NAME UnitTests COMMAND unit_testing)
