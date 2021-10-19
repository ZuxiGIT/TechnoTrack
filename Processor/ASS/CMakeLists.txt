cmake_minimum_required(VERSION 3.2)


add_library(ass_lib
    assembler.c)

target_link_libraries(ass_lib
    PUBLIC text)

add_executable(ass
    Ass_main.c)

target_link_libraries(ass
    PRIVATE ass_lib)