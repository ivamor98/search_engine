cmake_minimum_required(VERSION 3.10.0)
project(search_engine VERSION 0.1.0 LANGUAGES C CXX)
set(HEADERS ${CMAKE_SOURCE_DIR}/include)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/Debug/
)

add_subdirectory(src)






