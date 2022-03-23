cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

include(CTest)

macro(create_test name)
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

# FLAGS NECESSARY FOR GUMBO
set(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/local/include")
set(GCC_COVERAGE_LINK_FLAGS    "-L/usr/local/lib -lgumbo")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")


#---------------------------------------------------------------------------------------------------------
# DEFINITIONS
add_definitions(-DSEARCHENGINE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")

add_definitions(-DDOCUMENT_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/document-crawler\")
add_definitions(-DDOCUMENT_CRAWLER_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/document-crawler/testing\")

add_definitions(-DWEB_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/web-crawler\")
add_definitions(-DWEB_CRAWLER_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/web-crawler/testing\")

add_definitions(-DINDEXING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing\")
add_definitions(-DINDEXING_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing/testing\")

add_definitions(-DRANKING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking\")
add_definitions(-DRANKING_TESTING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking/testing\")


#---------------------------------------------------------------------------------------------------------
# PACKAGES
find_package(nlohmann_json CONFIG REQUIRED)


#---------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
add_subdirectory(testing)
add_subdirectory(modules)


#---------------------------------------------------------------------------------------------------------
# EXECUTABLES

# ON-THE-FLY -- BASIC RANKING -- DOCUMENT CRAWLER
add_executable(onTheFlyBasicDocument src/onTheFlyBasicDocument.cpp)

target_include_directories(onTheFlyBasicDocument
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(onTheFlyBasicDocument
    PRIVATE modules/crawling/document-crawler modules/indexing
)

target_link_libraries(onTheFlyBasicDocument
    PRIVATE document-crawler 
    PRIVATE indexing
    PRIVATE ranking
    PUBLIC nlohmann_json::nlohmann_json
)

# ON-THE-FLY -- BASIC RANKING -- WEB CRAWLER
add_executable(onTheFlyBasicWeb src/onTheFlyBasicWeb.cpp)

target_include_directories(onTheFlyBasicWeb
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(onTheFlyBasicWeb
    PRIVATE modules/crawling/web-crawler
)

target_link_libraries(onTheFlyBasicWeb
    PRIVATE web-crawler 
    PUBLIC nlohmann_json::nlohmann_json
)