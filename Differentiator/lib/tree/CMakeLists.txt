cmake_minimum_required(VERSION 3.2)

message("tree")

add_library(tree
    tree.c)

target_link_libraries(tree
    PRIVATE logger
    PRIVATE file)
