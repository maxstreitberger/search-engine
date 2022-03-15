cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

add_definitions(-DSEARCHENGINE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")

add_definitions(-DCRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling\")
add_definitions(-DCRAWLER_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/testing\")

add_definitions(-DINDEXING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing\")
add_definitions(-DINDEXING_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing/testing\")

add_definitions(-DRANKING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking\")
add_definitions(-DRANKING_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking/testing\")

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(testing) 
add_subdirectory(modules/crawling)
add_subdirectory(modules/indexing)
add_subdirectory(modules/ranking)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE modules/crawling modules/indexing
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE crawling 
    PRIVATE indexing
    PRIVATE ranking
    PUBLIC nlohmann_json::nlohmann_json
)
