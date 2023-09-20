add_library(Routes Routes.c)

add_subdirectory(Seed)
add_subdirectory(Payment)

target_link_libraries(Routes Map Http Response Models Logger Seed Payment)