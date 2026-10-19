include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS ArdoiseWidgets
    EXPORT ArdoiseTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

set(ardoise_package_dir "${CMAKE_INSTALL_LIBDIR}/cmake/Ardoise")
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/ArdoiseConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/ArdoiseConfig.cmake"
    INSTALL_DESTINATION "${ardoise_package_dir}"
)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/ArdoiseConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion
)

install(EXPORT ArdoiseTargets
    FILE ArdoiseTargets.cmake
    NAMESPACE Ardoise::
    DESTINATION "${ardoise_package_dir}"
)
install(FILES
    "${PROJECT_BINARY_DIR}/ArdoiseConfig.cmake"
    "${PROJECT_BINARY_DIR}/ArdoiseConfigVersion.cmake"
    DESTINATION "${ardoise_package_dir}"
)
