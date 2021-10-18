cmake_minimum_required(VERSION 3.2)

message("stack")
add_library(stack INTERFACE)

target_include_directories(stack
    INTERFACE .)

message("stk_print")
add_library(stk_print
    Print.c)    

target_link_libraries(stack 
    INTERFACE stk_print
    INTERFACE logger)