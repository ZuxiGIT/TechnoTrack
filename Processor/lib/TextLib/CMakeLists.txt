cmake_minimum_required(VERSION 3.2)

message("file")

add_library(file
    File.c)

message("strings")

add_library(strings
    Strings.c)

message("text")

add_library(text
    Text.c)

target_link_libraries(file
    PRIVATE strings
    PUBLIC text)

target_link_libraries(text
    PUBLIC strings
    PRIVATE file)