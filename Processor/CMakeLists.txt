cmake_minimum_required(VERSION 3.2)

project(SoftProccessot VERSION 1.0 LANGUAGES C  )

add_subdirectory(lib EXCLUDE_FROM_ALL)

add_executable(proc EXCLUDE_FROM_ALL
    CPU/proc.c
)


add_subdirectory(ASS EXCLUDE_FROM_ALL)


add_executable(ass EXCLUDE_FROM_ALL
    ASS/Ass_main.c)

target_link_libraries(ass
    PRIVATE ass_lib
    PRIVATE logger
    PRIVATE text)


add_subdirectory(CPU EXCLUDE_FROM_ALL)

target_link_libraries(proc 
    PRIVATE proc_lib
    PRIVATE stack
    PRIVATE logger
    PRIVATE text
)