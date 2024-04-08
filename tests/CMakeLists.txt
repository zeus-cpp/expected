if (MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/W4)
endif ()

add_subdirectory(test_expected)
add_subdirectory(third_party)
