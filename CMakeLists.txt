cmake_minimum_required(VERSION 3.21)

project(Ardoise VERSION 0.1.0 LANGUAGES CXX)

option(ARDOISE_BUILD_EXAMPLES "Build the integration example" ${PROJECT_IS_TOP_LEVEL})
option(ARDOISE_INSTALL "Enable library installation and package exports" ${PROJECT_IS_TOP_LEVEL})

find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets Svg)
include(GNUInstallDirs)

add_subdirectory(src)

if(ARDOISE_INSTALL)
    include(cmake/InstallArdoise.cmake)
endif()

if(ARDOISE_BUILD_EXAMPLES)
    add_subdirectory(examples/consumer)
    add_subdirectory(examples/gallery)
endif()

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
