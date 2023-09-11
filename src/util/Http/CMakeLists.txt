add_library(Http http.c)

target_link_libraries(Http Cstrings Map JsonParser TCP Logger Response)

add_subdirectory(Response)
