cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name
set(TARGET "oled-driver")
set(LIB_TARGET ${TARGET} PARENT_SCOPE)
project(${TARGET} CXX)

# Source and header files to build
set(
    SOURCES
	"Graphics.h"
	"Graphics.cpp"
	"Drivers/ssd1351.h"
	"Drivers/ssd1351.cpp"
    "Serial/ISerial.h"
)

# Keep structure for Visual Studio
assign_source_group(${SOURCES})

# Export dir for include
set(LIB_INC ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

# Build this as a library
add_library(${TARGET} ${SOURCES})
