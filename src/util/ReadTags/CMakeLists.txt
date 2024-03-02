add_library(ReadTags ReadTags.c)
#target_link_libraries(ReadTags Cstrings Map ReadRFIDTags)
target_link_libraries(ReadTags Cstrings Map)