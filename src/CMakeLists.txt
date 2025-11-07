cmake_minimum_required(VERSION 3.5.0)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_executable(search_engine main.cpp   
                            ConverterJSON.cpp
                            InvertedIndex.cpp
                            SearchServer.cpp
                            ${HEADERS}/ConverterJSON.h
                            ${HEADERS}/InvertedIndex.h
                            ${HEADERS}/SearchServer.h)
include(FetchContent)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json/ GIT TAG v3.11.3)

FetchContent_MakeAvailable(json)


target_include_directories(search_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)


target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)