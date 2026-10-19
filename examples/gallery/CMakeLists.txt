add_executable(ardoise-gallery main.cpp)
target_link_libraries(ardoise-gallery PRIVATE Ardoise::Widgets)

if(MSVC)
    target_compile_options(ardoise-gallery PRIVATE /utf-8)
endif()
