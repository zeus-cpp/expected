project(test_expected)

# [=== C++17 ===]

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} zeus::expected)

find_package(Catch2 REQUIRED)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)

target_sources(${PROJECT_NAME}
    PRIVATE
        base_tests.cpp
        test_expected_main.cpp
)

# [=== C++20 ===]

add_executable(${PROJECT_NAME}_cpp20)
set_target_properties(${PROJECT_NAME}_cpp20 PROPERTIES CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME}_cpp20 zeus::expected)

find_package(Catch2 REQUIRED)
target_link_libraries(${PROJECT_NAME}_cpp20 Catch2::Catch2)

target_sources(${PROJECT_NAME}_cpp20
    PRIVATE
        base_tests.cpp
        test_expected_main.cpp
)

# [=== C++23 ===]

add_executable(${PROJECT_NAME}_cpp23)
set_target_properties(${PROJECT_NAME}_cpp23 PROPERTIES CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}_cpp23 zeus::expected)

find_package(Catch2 REQUIRED)
target_link_libraries(${PROJECT_NAME}_cpp23 Catch2::Catch2)

target_sources(${PROJECT_NAME}_cpp23
    PRIVATE
        base_tests.cpp
        test_expected_main.cpp
)
