cmake_minimum_required(VERSION 3.2)

project(SoftProccessot VERSION 1.0 LANGUAGES C  )

add_subdirectory(lib EXCLUDE_FROM_ALL)

add_executable(myExe 
    proc.c
)

target_link_libraries(myExe
    PRIVATE stack
    PRIVATE logger
    PRIVATE text
)
    # ./lib/Stack/Print.c
    # ./lib/TextLib/Text.c
    # ./lib/logger/logger.c
    # ./lib/TextLib/File.c
    # ./lib/TextLib/Strings.c)