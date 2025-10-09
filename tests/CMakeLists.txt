cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 20)
project(search_engine_test VERSION 0.1.0)
add_executable(search_engine_test 
                            ${SRC}/ConverterJSON.cpp 
                            ${SRC}/InvertedIndex.cpp 
                            ${SRC}/SearchServer.cpp
                            ${HEADERS}/ConverterJSON.h 
                            ${HEADERS}/InvertedIndex.h 
                            ${HEADERS}/SearchServer.h
                            InvertedIndexTests.cpp 
                            )

include(FetchContent)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json/ GIT TAG v3.11.3)
FetchContent_MakeAvailable(json)
target_link_libraries(search_engine_test PUBLIC nlohmann_json::nlohmann_json)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(search_engine_test PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine_test)

target_include_directories(search_engine_test PUBLIC ${HEADERS})