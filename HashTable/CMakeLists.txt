cmake_minimum_required(VERSION 3.2)

project(HashTable VERSION 1.0 LANGUAGES C  )

add_compile_options(-ggdb -Wall -Wextra -Werror -fno-omit-frame-pointer)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_subdirectory(lib)

add_library(hashtable
    hashtable.c
)

add_executable(main
    main.c
)

target_link_libraries(hashtable
    PRIVATE list 
)

target_link_libraries(main
    PRIVATE hashtable
)

