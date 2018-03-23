cmake_minimum_required (VERSION 2.6)

project (CHIP8EMU)
set (CHIP8EMU_VERSION_MAJOR 1)
set (CHIP8EMU_VERSION_MINOR 0)

set (USE_DEBUG FALSE)

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
if(NOT SFML_ROOT)
	set(SFML_ROOT "C:/" CACHE PATH "Where is SFML located?")
endif()

# Detect and add SFML
#set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

set(EXECUTABLE_NAME "CHIP8EMU")
set(SOURCE_FILES
	MainF.cpp
	CHIP8App.cpp
	src/Chip8.cpp
	src/Options.cpp
	src/System.cpp
	src/wxSFMLCanvas.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} resource.rc)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# Note that for MinGW users the order of libs is important!
find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
add_definitions(-DWXUSINGDLL -DwxUSE_UNICODE)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}/sfml")

configure_file(beep.wav "${CMAKE_BINARY_DIR}/beep.wav" COPYONLY)

if(USE_DEBUG)
	add_definitions(-DWRITE_DEBUG)
	configure_file(debug.ttf "${CMAKE_BINARY_DIR}/debug.ttf" COPYONLY)
endif()