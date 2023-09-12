add_executable(Api Api.c)

target_link_libraries(Api Http socket App Routes)

add_subdirectory(Router)
add_subdirectory(App)
add_subdirectory(Routes)