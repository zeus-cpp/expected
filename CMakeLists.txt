cmake_minimum_required(VERSION 3.23)

project(zeus_expected)

add_library(${PROJECT_NAME} INTERFACE)
add_library(zeus::expected ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_sources(${PROJECT_NAME} PUBLIC FILE_SET exports TYPE HEADERS BASE_DIRS include FILES
    include/zeus/expected.hpp
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    FILE_SET exports
)
install(EXPORT ${PROJECT_NAME}-targets DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    FILE ${PROJECT_NAME}.cmake
    COMPONENT Development EXCLUDE_FROM_ALL
)

add_subdirectory(tests)
