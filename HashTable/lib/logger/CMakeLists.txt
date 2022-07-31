cmake_minimum_required(VERSION 3.2)

message("logger")

add_library(logger
    logger.c)

add_library(TColors INTERFACE)

target_include_directories(TColors
    INTERFACE .)

target_link_libraries(logger
    PRIVATE TColors)
