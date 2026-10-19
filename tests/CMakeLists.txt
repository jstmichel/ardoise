configure_file(RunConsumer.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/RunConsumer.cmake" @ONLY)

add_test(NAME ardoise-source-consumer
    COMMAND "${CMAKE_COMMAND}"
        -DMODE=source
        "-DCONFIG=$<CONFIG>"
        -P "${CMAKE_CURRENT_BINARY_DIR}/RunConsumer.cmake"
)

if(ARDOISE_INSTALL)
    add_test(NAME ardoise-installed-consumer
        COMMAND "${CMAKE_COMMAND}"
            -DMODE=installed
            "-DCONFIG=$<CONFIG>"
            -P "${CMAKE_CURRENT_BINARY_DIR}/RunConsumer.cmake"
    )
endif()

find_package(Qt6 6.2 REQUIRED COMPONENTS Test)
add_executable(ardoise-theme-test ThemeTest.cpp)
set_target_properties(ardoise-theme-test PROPERTIES AUTOMOC ON)
target_link_libraries(ardoise-theme-test PRIVATE Ardoise::Widgets Qt6::Test)
add_test(NAME ardoise-theme-test COMMAND ardoise-theme-test)
set_tests_properties(ardoise-theme-test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
add_executable(ardoise-widgets-test WidgetsTest.cpp)
set_target_properties(ardoise-widgets-test PROPERTIES AUTOMOC ON)
target_link_libraries(ardoise-widgets-test PRIVATE Ardoise::Widgets Qt6::Test)
add_test(NAME ardoise-widgets-test COMMAND ardoise-widgets-test)
set_tests_properties(ardoise-widgets-test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
add_executable(ardoise-window-test WindowTest.cpp)
set_target_properties(ardoise-window-test PROPERTIES AUTOMOC ON)
target_link_libraries(ardoise-window-test PRIVATE Ardoise::Widgets Qt6::Test)
add_test(NAME ardoise-window-test COMMAND ardoise-window-test)
set_tests_properties(ardoise-window-test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

if(MSVC)
    target_compile_options(ardoise-theme-test PRIVATE /utf-8)
    target_compile_options(ardoise-widgets-test PRIVATE /utf-8)
    target_compile_options(ardoise-window-test PRIVATE /utf-8)
endif()
