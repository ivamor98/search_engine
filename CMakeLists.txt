cmake_minimum_required(VERSION 3.10.0)
project(search_engine VERSION 0.1.0 LANGUAGES C CXX)
set(HEADERS ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/SRC)
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(tests)






