
add_executable(Cstrings_Test cstrings_test.c)

target_link_libraries(Cstrings_Test Cstrings)

add_test(CstringsTest Cstrings_Test)
