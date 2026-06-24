cmake_minimum_required(VERSION 3.23)

project(
    zeus_expected
    VERSION 1.3.3
    DESCRIPTION "Backporting std::expected to C++17."
    HOMEPAGE_URL "https://github.com/zeus-cpp/expected"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)
add_library(zeus::expected ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET exports
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/zeus/expected.hpp
)

set_target_properties(zeus_expected PROPERTIES
    EXPORT_NAME expected
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

option(ZEUS_EXPECTED_INSTALL "Generate install targets" ${PROJECT_IS_TOP_LEVEL})
option(ZEUS_EXPECTED_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})

if(ZEUS_EXPECTED_INSTALL)
    include(GNUInstallDirs)

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        FILE_SET exports
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT Development
    )

    install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE zeus::
        DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
        FILE ${PROJECT_NAME}Targets.cmake
        COMPONENT Development
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
        COMPONENT Development
    )
endif()

if(ZEUS_EXPECTED_BUILD_TESTS)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()
endif ()
