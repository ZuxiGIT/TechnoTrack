cmake_minimum_required(VERSION 3.2)

project(SoftProccessot VERSION 1.0 LANGUAGES C  )

add_library(list
    list.c)

add_subdirectory(logger)

target_link_libraries(list
    PRIVATE logger
)

add_executable(main
    main.c
)

target_link_libraries(main
    PRIVATE list
)
