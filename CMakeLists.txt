cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DSEARCHENGINE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DCRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling\")
add_definitions(-DCRAWLER_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/testing\")

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(testing) 
add_subdirectory(modules/crawling)
add_subdirectory(modules/indexing)
add_subdirectory(modules/ranking)

target_link_directories(${PROJECT_NAME} PRIVATE modules/crawling modules/indexing)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE crawling 
    PRIVATE indexing
    PRIVATE ranking
)
