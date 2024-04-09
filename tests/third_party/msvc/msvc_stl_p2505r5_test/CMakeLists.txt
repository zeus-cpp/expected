project(msvc_stl_p2505r5_test)
add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} zeus::expected)

target_sources(${PROJECT_NAME}
    PRIVATE
        msvc_stl_p2505r5_test.cpp
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
