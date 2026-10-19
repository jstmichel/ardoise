cmake_minimum_required(VERSION 3.21)

project(ArdoiseConsumer LANGUAGES CXX)

if(NOT TARGET Ardoise::Widgets)
    set(ARDOISE_SOURCE_DIR "" CACHE PATH "Ardoise source checkout; empty uses the installed package")
    if(ARDOISE_SOURCE_DIR)
        add_subdirectory("${ARDOISE_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/ardoise" EXCLUDE_FROM_ALL)
    else()
        find_package(Ardoise 0.1 CONFIG REQUIRED)
    endif()
endif()

add_executable(ardoise-consumer main.cpp)
target_link_libraries(ardoise-consumer PRIVATE Ardoise::Widgets)
set_target_properties(ardoise-consumer PROPERTIES CXX_EXTENSIONS OFF)
