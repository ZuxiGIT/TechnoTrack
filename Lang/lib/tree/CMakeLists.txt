cmake_minimum_required(VERSION 3.2)

message("tree")

add_library(tree
    _tree.c)

target_link_libraries(tree
    PUBLIC id_table
    PRIVATE logger
    PRIVATE file)
