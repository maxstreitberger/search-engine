cmake_minimum_required(VERSION 3.21.3)

project(seeker VERSION 1.0)
configure_file(cmake/SeekerConfig.h.in cmake/SeekerConfig.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-linux/share")
endif()

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")
endif()

option(SEEKER_INSTALL_DOCS "Install documentation alongside library" ON)
option(SEEKER_DEVELOPMENT_BUILD "Build tests, enable warnings" OFF)
option(SEEKER_BENCHMARK_BUILD "Build benchmarks" OFF)
option(SEEKER_WARNINGS_AS_ERRORS "Enables the -WError compiler flag" OFF)

set(SEEKER_NORMAL_COMPILER_FLAGS "-O2 -w")
set(SEEKER_DEVELOPMENT_COMPILER_FLAGS "--coverage -g -O0 -Wall -pedantic-errors -Wextra -Wconversion -Wsign-conversion")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${SEEKER_NORMAL_COMPILER_FLAGS})
set(SEEKER_NORMAL_COMPILER_FLAGS "-I/usr/local/include -O2 -w -pthread")
set(SEEKER_DEVELOPMENT_COMPILER_FLAGS "-I/usr/local/include --coverage -g -O0 -Wall -pedantic-errors -Wextra -Wconversion -Wsign-conversion")

set(CMAKE_CXX_FLAGS ${SEEKER_NORMAL_COMPILER_FLAGS})


#---------------------------------------------------------------------------------------------------------
# DEFINITIONS
add_definitions(-DSEARCHENGINE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DSEARCHENGINE_TESTING_DIR=\"${CMAKE_SOURCE_DIR}/testing\")
add_definitions(-DSEARCHENGINE_BENCHMARKING_DIR=\"${CMAKE_SOURCE_DIR}/benchmarking\")

add_definitions(-DDOCUMENT_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/document-crawler\")
add_definitions(-DWEB_CRAWLER_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/crawling/web-crawler\")
add_definitions(-DINDEXING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/indexing\")
add_definitions(-DRANKING_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/modules/ranking\")


#---------------------------------------------------------------------------------------------------------
# PACKAGES
find_package(CURL REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(unofficial-gumbo CONFIG REQUIRED)


#---------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
add_subdirectory(modules)


#---------------------------------------------------------------------------------------------------------
# DEVELOPMENT
if (SEEKER_DEVELOPMENT_BUILD)
    set(CMAKE_CXX_FLAGS ${SEEKER_DEVELOPMENT_COMPILER_FLAGS})
    find_package(Catch2 CONFIG REQUIRED)
    include(CTest)
    macro(create_test name)
        add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
    endmacro()
    
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

    add_subdirectory(testing)
if (SEEKER_BENCHMARK_BUILD)
    message("Development 🧪")
    set(CMAKE_CXX_FLAGS ${SEEKER_NORMAL_COMPILER_FLAGS})
    add_subdirectory(benchmarking)
endif()


#---------------------------------------------------------------------------------------------------------
# EXECUTABLES

# ON-THE-FLY -- DOCUMENT CRAWLER
add_executable(OnTheFlyDocumentSearch src/onTheFlyDocumentSearch.cpp)

target_include_directories(OnTheFlyDocumentSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OnTheFlyDocumentSearch
    PRIVATE DocumentCrawler
    PRIVATE Indexer
    PRIVATE Ranker
)

# PRE-COMPUTED -- DOCUMENT CRAWLER
add_executable(PreComputedDocumentSearch src/preComputedBasicDocument.cpp)

target_include_directories(PreComputedDocumentSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(PreComputedDocumentSearch
    PRIVATE PreComputedDocumentCrawler
    PRIVATE PreComputedDocumentStore
    PRIVATE PreComputedIndexer
    PRIVATE Ranker
)

# CONCURRENT -- DOCUMENT CRAWLER
add_executable(ConcurrentDocumentSearch src/concurrentDocumentSearch.cpp)

target_include_directories(ConcurrentDocumentSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ConcurrentDocumentSearch
    PRIVATE ConcurrentDocumentCrawler
    PRIVATE ConcurrentDocumentStore
    PRIVATE ConcurrentIndexer
    PRIVATE Ranker
)