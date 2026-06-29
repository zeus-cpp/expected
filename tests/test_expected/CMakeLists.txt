project(test_expected LANGUAGES CXX)

set(SOURCES
    base_tests.cpp
    monadic_tests.cpp
    noexcept_tests.cpp
    equality_tests.cpp
    lwg_3886_tests.cpp
    lwg_4031_tests.cpp
    lwg_4222_tests.cpp
)

find_package(Catch2 3 REQUIRED)

include(Catch)

function(add_test_expected CPP_STANDARD)
    set(TARGET_NAME ${PROJECT_NAME}_cpp${CPP_STANDARD})

    add_executable(${TARGET_NAME})
    set_target_properties(${TARGET_NAME}
        PROPERTIES CXX_STANDARD ${CPP_STANDARD})
    # The two targets belong to different scopes,
    # so linking them separately is better for clarity.
    target_link_libraries(${TARGET_NAME}
        PRIVATE Catch2::Catch2WithMain)
    target_link_libraries(${TARGET_NAME}
        PRIVATE zeus::expected)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})

    catch_discover_tests(${TARGET_NAME})
endfunction()

add_test_expected(17)

if (cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    add_test_expected(20)
endif ()

if (cxx_std_23 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    add_test_expected(23)
endif ()
