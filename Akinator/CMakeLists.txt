cmake_minimum_required(VERSION 3.2)

project(Akinator VERSION 1.0 LANGUAGES C  )


add_subdirectory(lib)

add_executable(main
    main.c
)

target_link_libraries(main
    PRIVATE tree
    PRIVATE stack
    PRIVATE logger
    PRIVATE file
    PRIVATE strings
    PRIVATE text
)
