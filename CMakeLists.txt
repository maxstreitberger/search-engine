cmake_minimum_required(VERSION 3.21.3)

project(search-engine)

include(CTest)

macro(create_test name)
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug mode")
    set(CMAKE_CXX_FLAGS "-std=c++0x -Wall")
    set(CMAKE_CXX_FLAGS "-I/usr/local/include --coverage -g -O0 -Wall")
else()
    message("Release mode")
    set(CMAKE_CXX_FLAGS "-I/usr/local/include -O2")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lgumbo")

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
find_package(CURL REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

#---------------------------------------------------------------------------------------------------------
# SUB-DIRECTORIES
add_subdirectory(testing)
add_subdirectory(modules)
add_subdirectory(benchmarking)


#---------------------------------------------------------------------------------------------------------
# EXECUTABLES

# ON-THE-FLY -- DOCUMENT CRAWLER
add_executable(OnTheFlyDocumentSearch src/onTheFlyDocumentSearch.cpp)
set_property(TARGET OnTheFlyDocumentSearch PROPERTY VERSION "1.0.0")
configure_file(OnTheFlyDocumentSearch.h.in OnTheFlyDocumentSearch.h)

target_include_directories(OnTheFlyDocumentSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OnTheFlyDocumentSearch
    PRIVATE document-crawler
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE glog::glog
)

install(TARGETS OnTheFlyDocumentSearch DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/OnTheFlyDocumentSearch.h DESTINATION include)

# ON-THE-FLY -- WEB CRAWLER
add_executable(OnTheFlyWebSearch src/onTheFlyWebSearch.cpp)
set_property(TARGET OnTheFlyWebSearch PROPERTY VERSION "1.0.0")
configure_file(OnTheFlyWebSearch.h.in OnTheFlyWebSearch.h)

target_include_directories(OnTheFlyWebSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OnTheFlyWebSearch
    PRIVATE web-crawler 
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE glog::glog
)

install(TARGETS OnTheFlyWebSearch DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/OnTheFlyWebSearch.h DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)