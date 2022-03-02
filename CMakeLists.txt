cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(testing) 
add_subdirectory(modules/indexing)

target_link_directories(${PROJECT_NAME} PRIVATE modules/indexing)
target_link_libraries(${PROJECT_NAME} indexing)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dummy-text
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})