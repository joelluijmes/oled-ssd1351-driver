cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(ssd1351-framework)

# organize using folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

# function to organize source files into original structure (Visual Studio)
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

add_subdirectory(oled-driver)
add_subdirectory(demo/win/serial-lib)
add_subdirectory(demo/win/test)
