cmake_minimum_required(VERSION 3.0.0)
project(SmartSupermarket VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_subdirectory(util)
add_subdirectory(Apps)
add_subdirectory(libs)

add_library(socket socket.c)
add_library(childProcess childProcess.c)

add_executable(server server.c)

target_link_libraries(server socket)
target_link_libraries(socket childProcess)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
