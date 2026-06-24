project(test_no_exceptions LANGUAGES CXX)

add_executable(${PROJECT_NAME} test_no_exceptions.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE zeus::expected)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /EHs-c-)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _HAS_EXCEPTIONS=0)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-exceptions)
endif ()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
