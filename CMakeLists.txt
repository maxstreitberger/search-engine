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
set(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/local/include --coverage -g -O0")
set(GCC_COVERAGE_LINK_FLAGS    "-L/usr/local/lib -lgumbo")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

#---------------------------------------------------------------------------------------------------------
# DEFINITIONS
add_definitions(-DSEARCHENGINE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DSEARCHENGINE_TESTING_DIR=\"${CMAKE_SOURCE_DIR}/testing\")

add_definitions(-DDOCUMENT_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/document-crawler\")
add_definitions(-DWEB_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/web-crawler\")
add_definitions(-DINDEXING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing\")
add_definitions(-DRANKING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking\")


#---------------------------------------------------------------------------------------------------------
# PACKAGES
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(lyra CONFIG REQUIRED)


#---------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
add_subdirectory(testing)
add_subdirectory(modules)
add_subdirectory(benchmarking)


#---------------------------------------------------------------------------------------------------------
# EXECUTABLES

# ON-THE-FLY -- BASIC RANKING -- DOCUMENT CRAWLER
add_executable(onTheFlyBasicDocument src/onTheFlyBasicDocument.cpp)

target_include_directories(onTheFlyBasicDocument
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(onTheFlyBasicDocument
    PRIVATE modules/crawling/document-crawler
    PRIVATE modules/crawling/document-store
    PRIVATE modules/indexing
    PRIVATE modules/ranking
)

target_link_libraries(onTheFlyBasicDocument
    PRIVATE document-crawler
    PRIVATE doc_store
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE bfg::lyra
)

# ON-THE-FLY -- BASIC RANKING -- WEB CRAWLER
add_executable(onTheFlyBasicWeb src/onTheFlyBasicWeb.cpp)

target_include_directories(onTheFlyBasicWeb
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(onTheFlyBasicWeb
    PRIVATE modules/crawling/web-crawler
    PRIVATE modules/indexing
    PRIVATE modules/ranking
)

target_link_libraries(onTheFlyBasicWeb
    PRIVATE web-crawler 
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE glog::glog
    PRIVATE bfg::lyra
)

# PRE-COMPUTED -- BASIC RANKING -- DOCUMENT CRAWLER
add_executable(preComputedBasicDocument src/preComputedBasicDocument.cpp)

target_include_directories(preComputedBasicDocument
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(preComputedBasicDocument
    PRIVATE modules/crawling/pre-computed-document-crawler
    PRIVATE modules/crawling/pre-computed-document-store
    PRIVATE modules/pre-computed-indexer
    PRIVATE modules/ranking
)

target_link_libraries(preComputedBasicDocument
    PRIVATE pre-computed-document-crawler
    PRIVATE pre_computed_doc_store
    PRIVATE pre-computed-indexer
    PRIVATE ranking
    PRIVATE glog::glog
    PRIVATE bfg::lyra
)