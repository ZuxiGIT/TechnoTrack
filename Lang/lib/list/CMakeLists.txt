cmake_minimum_required(VERSION 3.2)

add_library(list
    list.c)

target_link_libraries(list
    PRIVATE logger
)
