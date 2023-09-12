add_library(Router Router.c)
add_library(RouterConfig RouterConfig.c)

target_link_libraries(Router Logger RouterConfig Http Response)
target_link_libraries(RouterConfig Map)