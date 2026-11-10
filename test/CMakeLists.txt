cmake_minimum_required(VERSION 3.16)
project(strpp_test LANGUAGES C)

add_library(strpp STATIC
    ../strpp.c
)

add_executable(strpp_test test.c)

target_include_directories(strpp_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(strpp_test PRIVATE strpp)
