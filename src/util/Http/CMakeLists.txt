add_library(Http http.c)

target_link_libraries(Http Cstrings Map JsonParser TCP Logger)

add_subdirectory(Response)
