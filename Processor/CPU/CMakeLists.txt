cmake_minimum_required(VERSION 3.2)


add_library(proc_lib
    cpu.c)

target_link_libraries(proc_lib
    PRIVATE stack)