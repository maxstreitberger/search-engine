cmake_minimum_required(VERSION 3.21.3)

project(seeker VERSION 1.0)
configure_file(cmake/SeekerConfig.h.in cmake/SeekerConfig.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

option(SEEKER_INSTALL_DOCS "Install documentation alongside library" ON)
option(SEEKER_DEVELOPMENT_BUILD "Build tests and benchmarks, enable warnings" OFF)
option(SEEKER_WARNINGS_AS_ERRORS "Enables the -WError compiler flag" OFF)

set(SEEKER_NORMAL_COMPILER_FLAGS "-I/usr/local/include -O2 -w")
set(SEEKER_DEVELOPMENT_COMPILER_FLAGS "-I/usr/local/include --coverage -g -O0 -Wall -pedantic-errors -Wextra -Wconversion -Wsign-conversion")

set(CMAKE_CXX_FLAGS ${SEEKER_NORMAL_COMPILER_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/lib -lgumbo")


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
add_subdirectory(modules)


#---------------------------------------------------------------------------------------------------------
# DEVELOPMENT
if (SEEKER_DEVELOPMENT_BUILD)
    message("Testing 🧪")
    set(CMAKE_CXX_FLAGS ${SEEKER_DEVELOPMENT_COMPILER_FLAGS})
    include(CTest)
    macro(create_test name)
        add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
    endmacro()
    
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

    add_subdirectory(testing)
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
    PRIVATE glog::glog
)

# ON-THE-FLY -- WEB CRAWLER
add_executable(OnTheFlyWebSearch src/onTheFlyWebSearch.cpp)

target_include_directories(OnTheFlyWebSearch
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(OnTheFlyWebSearch
    PRIVATE WebCrawler 
    PRIVATE Indexer
    PRIVATE Ranker
    PRIVATE glog::glog
)


#---------------------------------------------------------------------------------------------------------
# CPACK

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Seeker")
set(CPACK_PACKAGE_VENDOR "Maximilian Streitberger")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Modular search engine")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)


#---------------------------------------------------------------------------------------------------------
# INSTALLATION LIBRARY

include(GNUInstallDirs)
set(SEEKER_CMAKE_CONFIG_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Seeker)
set(SEEKER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DOCUMENT_CRAWLER_PATH ${SEEKER_DIR}/modules/crawling/document-crawler)
set(WEB_CRAWLER_PATH ${SEEKER_DIR}/modules/crawling/web-crawler)
set(DOCUMENT_STORE_PATH ${SEEKER_DIR}/modules/crawling/document-store)
set(INDEXER_PATH ${SEEKER_DIR}/modules/indexing)
set(RANKER_PATH ${SEEKER_DIR}/modules/ranking)

set(SEEKER_HEADERS
    ${SEEKER_DIR}/include/doc_meta.hpp
    ${SEEKER_DIR}/include/token_meta.hpp
    ${SEEKER_DIR}/include/engine.hpp
    ${SEEKER_DIR}/include/helpers.hpp
)

set(DOCUMENT_CRAWLER_SOURCES
    ${DOCUMENT_CRAWLER_PATH}/src/doc_crawler.cpp
)

set(DOCUMENT_CRAWLER_HEADERS
    ${DOCUMENT_CRAWLER_PATH}/include/doc_crawler.hpp
)

set(DOCUMENT_CRAWLER_FILES ${DOCUMENT_CRAWLER_HEADERS} ${DOCUMENT_CRAWLER_SOURCES})

set(WEB_CRAWLER__SOURCES
    ${WEB_CRAWLER_PATH}/src/web_crawler.cpp
)

set(WEB_CRAWLER_HEADERS
    ${WEB_CRAWLER_PATH}/include/web_crawler.hpp
)

set(WEB_CRAWLER_FILES ${WEB_CRAWLER_HEADERS} ${WEB_CRAWLER__SOURCES})

set(DOCUMENT_STORE_SOURCES
    ${DOCUMENT_STORE_PATH}/src/doc_store.cpp
)

set(DOCUMENT_STORE_HEADERS
    ${DOCUMENT_STORE_PATH}/include/doc_store.hpp
)

set(DOCUMENT_STORE_FILES ${DOCUMENT_STORE_HEADERS} ${DOCUMENT_STORE_SOURCES})


set(INDEXER_SOURCES
    ${INDEXER_PATH}/src/indexer.cpp
)

set(INDEXER_HEADERS
    ${INDEXER_PATH}/include/indexer.hpp
)

set(INDEXER_FILES ${INDEXER_HEADERS} ${INDEXER_SOURCES})

set(RANKER_SOURCES
    ${RANKER_PATH}/src/ranker.cpp
)

set(RANKER_HEADERS
    ${RANKER_PATH}/include/ranker.hpp
)

set(RANKER_FILES ${RANKER_HEADERS} ${RANKER_SOURCES})

add_library(Seeker STATIC 
    ${DOCUMENT_CRAWLER_FILES}
    ${WEB_CRAWLER_FILES}
    ${DOCUMENT_STORE_FILES}
    ${INDEXER_FILES}
    ${RANKER_FILES}
)
add_library(Seeker::Seeker ALIAS Seeker)
set_target_properties(Seeker PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(Seeker
  PUBLIC
    $<BUILD_INTERFACE:${SEEKER_DIR}/include>
    $<BUILD_INTERFACE:${DOCUMENT_STORE_PATH}/include>
    $<BUILD_INTERFACE:${DOCUMENT_CRAWLER_PATH}/include>
    $<BUILD_INTERFACE:${WEB_CRAWLER_PATH}/include>
    $<BUILD_INTERFACE:${INDEXER_PATH}/include>
    $<BUILD_INTERFACE:${RANKER_PATH}/include>
    $<INSTALL_INTERFACE:include>
)

install(FILES 
    ${SEEKER_HEADERS}
    ${DOCUMENT_CRAWLER_HEADERS} 
    ${WEB_CRAWLER_HEADERS} 
    ${DOCUMENT_STORE_HEADERS} 
    ${INDEXER_HEADERS} 
    ${RANKER_HEADERS} 
    DESTINATION include/seeker
)

install(
    TARGETS
        Seeker
    EXPORT
        SeekerTargets
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
)

install(
    EXPORT
        SeekerTargets
    NAMESPACE
        Seeker::
    DESTINATION
        ${SEEKER_CMAKE_CONFIG_DESTINATION}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/SeekerConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/Seeker"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SeekerConfigVersion.cmake"
    VERSION "${Seeker_VERSION_MAJOR}.${Seeker_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SeekerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SeekerConfigVersion.cmake
    DESTINATION lib/cmake/Seeker
)


#---------------------------------------------------------------------------------------------------------
# INSTALLATION EXECUTABLES
install(TARGETS OnTheFlyWebSearch DESTINATION bin)
install(TARGETS OnTheFlyDocumentSearch DESTINATION bin)
