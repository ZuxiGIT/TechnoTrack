cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -g")
project(YASLI VERSION 1.0 LANGUAGES C)
add_subdirectory(lib)

add_executable(main
    main.c)

target_link_libraries(main
    INTERFACE TColors
    PRIVATE tree
    PRIVATE list
    PRIVATE id_table
    PRIVATE logger)
