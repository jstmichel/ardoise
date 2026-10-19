configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/VersionPrivate.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VersionPrivate.h"
    @ONLY
)

add_library(ArdoiseWidgets STATIC
    "${PROJECT_SOURCE_DIR}/include/Ardoise/Version.h"
    Version.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/TitleBar.h"
    widgets/TitleBar.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/MainWindow.h"
    widgets/MainWindow.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/Dialog.h"
    widgets/Dialog.cpp

    "${PROJECT_SOURCE_DIR}/resources/icons.qrc"
    icons/IconProvider.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/Panel.h"
    widgets/Panel.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/PrimaryButton.h"
    widgets/PrimaryButton.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/IconButton.h"
    widgets/IconButton.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/SearchField.h"
    widgets/SearchField.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/PlainTextEdit.h"
    widgets/PlainTextEdit.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/CollapsibleSection.h"
    widgets/CollapsibleSection.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/TreeView.h"
    widgets/TreeView.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/ThumbnailDelegate.h"
    widgets/ThumbnailDelegate.cpp
    "${PROJECT_SOURCE_DIR}/include/Ardoise/ThumbnailView.h"
    widgets/ThumbnailView.cpp

    "${PROJECT_SOURCE_DIR}/include/Ardoise/Style.h"
    theme/Theme.cpp
    theme/Style.cpp
    theme/StyleControls.cpp
)
add_library(Ardoise::Widgets ALIAS ArdoiseWidgets)

set_target_properties(ArdoiseWidgets PROPERTIES
    EXPORT_NAME Widgets
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

target_compile_features(ArdoiseWidgets PUBLIC cxx_std_20)
target_link_libraries(ArdoiseWidgets PUBLIC Qt6::Widgets PRIVATE Qt6::Svg)
target_include_directories(ArdoiseWidgets
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ArdoiseWidgets PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(ArdoiseWidgets PRIVATE /W4 /utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
qt_add_translations(ArdoiseWidgets
    TS_FILES "${PROJECT_SOURCE_DIR}/translations/ardoise_en.ts" "${PROJECT_SOURCE_DIR}/translations/ardoise_fr.ts"
    QM_FILES_OUTPUT_VARIABLE ARDOISE_TRANSLATIONS)
set(ardoise_translation_resource "<RCC><qresource prefix=\"/ardoise/translations\">")
foreach(qm IN LISTS ARDOISE_TRANSLATIONS)
    get_filename_component(name "${qm}" NAME)
    string(APPEND ardoise_translation_resource "<file alias=\"${name}\">${qm}</file>")
endforeach()
string(APPEND ardoise_translation_resource "</qresource></RCC>")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ardoise_translations.qrc" CONTENT "${ardoise_translation_resource}")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/qrc_ardoise_translations.cpp"
    COMMAND Qt6::rcc --name ardoise_translations --output "${CMAKE_CURRENT_BINARY_DIR}/qrc_ardoise_translations.cpp"
        "${CMAKE_CURRENT_BINARY_DIR}/ardoise_translations.qrc"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/ardoise_translations.qrc" ${ARDOISE_TRANSLATIONS}
    VERBATIM)
target_sources(ArdoiseWidgets PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/qrc_ardoise_translations.cpp")
