cmake_minimum_required(VERSION 3.0.0)
project(SmartSupermarket VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_subdirectory(util)
add_subdirectory(Apps)
add_subdirectory(libs)
add_subdirectory(Models)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
