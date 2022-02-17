cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src/adder)

target_link_directories(${PROJECT_NAME} PRIVATE adder/)
target_link_libraries(${PROJECT_NAME} adder)
