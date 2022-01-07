cmake_minimum_required(VERSION 3.2)

project(Differentiator VERSION 1.0 LANGUAGES C  )

add_subdirectory(lib)

add_library(DSL INTERFACE)

#target_include_directories(DSL
#    INTERFACE .
#)

target_link_libraries(DSL
    INTERFACE tree
)

add_library(diff
    diff.c
)

target_link_libraries(diff
    PRIVATE DSL
    PRIVATE tree
    PRIVATE logger
)

add_executable(main
    main.c
)

target_link_libraries(main
    PRIVATE tree
    PRIVATE logger
    PRIVATE diff
    PRIVATE DSL
    PRIVATE m
)
