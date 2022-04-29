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
# set(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/local/include --coverage -g -O0")
set(GCC_COVERAGE_LINK_FLAGS    "-L/usr/local/lib -lgumbo")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
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

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#---------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
# add_subdirectory(testing)
add_subdirectory(modules)
# add_subdirectory(benchmarking)


#---------------------------------------------------------------------------------------------------------
# EXECUTABLES

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
)












add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)