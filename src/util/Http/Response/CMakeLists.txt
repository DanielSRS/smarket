add_library(Response Response.c)

target_link_libraries(Response Logger Map Cstrings)

add_executable(ResponseTest Response.test.c)
target_link_libraries(ResponseTest Response TestSuite)
add_test(newResponseIsNotNull ResponseTest newResponseIsNotNull)
add_test(toStringResponse ResponseTest toStringResponse)

