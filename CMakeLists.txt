cmake_minimum_required(VERSION 3.11.0)
project(SEARCH)
find_package(OpenMP REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_executable(search
    ${PROJECT_SOURCE_DIR}/src/adv/advanced.c
    ${PROJECT_SOURCE_DIR}/src/adv/formula.c
    ${PROJECT_SOURCE_DIR}/src/adv/poly.c
    ${PROJECT_SOURCE_DIR}/src/adv/wrappers.c
    ${PROJECT_SOURCE_DIR}/src/advancedRunner.c
    ${PROJECT_SOURCE_DIR}/src/basic.c
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/preliminary.c
)

target_compile_options(search PRIVATE "-O3" "-Wall" "-Werror" "-pedantic" ${OpenMP_CXX_FLAGS})

add_subdirectory(${PROJECT_SOURCE_DIR}/factor64 EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/primesieve EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/libmontmul EXCLUDE_FROM_ALL)

target_include_directories(search
    PRIVATE
        ${PROJECT_SOURCE_DIR}/libdivide
        ${PROJECT_SOURCE_DIR}/libmontmul/src
        ${PROJECT_SOURCE_DIR}/primesieve/include
        ${PROJECT_SOURCE_DIR}/returnif/src
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/adv
)

target_link_libraries(search
    PRIVATE
        factor64
        montmul
        libprimesieve
        m
        ${OpenMP_CXX_FLAGS}
)
