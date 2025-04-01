project(test_expected)

set(SOURCES
    base_tests.cpp
    monadic_tests.cpp
    noexcept_tests.cpp
    equality_tests.cpp
    lwg_3886_tests.cpp
    test_expected_main.cpp
)

function(add_test_expected CPP_STANDARD)
    set(TARGET_NAME ${PROJECT_NAME}_cpp${CPP_STANDARD})

    add_executable(${TARGET_NAME})
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD ${CPP_STANDARD})

    target_link_libraries(${TARGET_NAME} zeus::expected)

    find_package(Catch2 REQUIRED)
    target_link_libraries(${TARGET_NAME} Catch2::Catch2)

    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

    include(Catch)
    catch_discover_tests(${TARGET_NAME})
endfunction()

add_test_expected(17)
if (cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    add_test_expected(20)
endif ()
if (cxx_std_23 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    add_test_expected(23)
endif ()
