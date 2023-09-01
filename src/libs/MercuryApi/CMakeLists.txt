add_library(ReadRFIDTags ReadRFIDTags.c)
target_link_libraries(Map)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D TMR_ENABLE_SERIAL_READER_ONLY=1 -g -fPIC")
include_directories(/home/tec502/mercuryapi-1.35.0.82/c/src/api)

target_link_libraries(ReadRFIDTags mercuryapi readline pthread)