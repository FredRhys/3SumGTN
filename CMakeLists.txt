#[[
In writing this file I collaborated with Claude to assist with clean-up.
I affirm that all AI-generated and co-created content underwent thorough review and
evaluation. The final output actually reflects my understanding, expertise, and
intended meaning. While AI assistance was instrumental in the process, I maintain
full responsibility for the content, its accuracy, and its presentation. This
disclosure is made in the spirit of transparency and to acknowledge the role of AI
in the creation process.

Claude pointed out that file prefixes don't need the prefix ${PROJECT_SOURCE_DIR}
in most cases, and bumped the CMake minimum version from 3.11 to 3.19. It also 
removed references to OpenMP for C++ and inserted C references instead.
]]

cmake_minimum_required(VERSION 3.19)
project(SEARCH LANGUAGES C CXX)
find_package(OpenMP REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_executable(search
    src/adv/advanced.c
    src/adv/formula.c
    src/adv/poly.c
    src/adv/wrappers.c
    src/advancedRunner.c
    src/basic.c
    src/main.c
    src/preliminary.c
)

target_compile_options(search PRIVATE "-O3" "-Wall" "-Werror" "-pedantic")

add_subdirectory(factor64 EXCLUDE_FROM_ALL)
add_subdirectory(primesieve EXCLUDE_FROM_ALL)
add_subdirectory(libmontmul EXCLUDE_FROM_ALL)

target_include_directories(search
    PRIVATE
        libdivide
        libmontmul/src
        primesieve/include
        returnif/src
        src
        src/adv
)

target_link_libraries(search
    PRIVATE
        factor64
        montmul
        libprimesieve
        m
        OpenMP::OpenMP_C
)
