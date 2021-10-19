cmake_minimum_required(VERSION 3.2)

project(SoftProccessot VERSION 1.0 LANGUAGES C  )

add_subdirectory(lib EXCLUDE_FROM_ALL)

add_executable(proc EXCLUDE_FROM_ALL
    proc.c
)


add_subdirectory(ASS EXCLUDE_FROM_ALL)

target_link_libraries(proc
    PRIVATE stack
    PRIVATE logger
    PRIVATE text
)