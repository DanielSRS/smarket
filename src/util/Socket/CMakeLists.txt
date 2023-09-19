add_library(socket socket.c)

target_link_libraries(socket childProcess Http TCP Logger)
