project(DocumentCrawler)

add_library(${PROJECT_NAME} STATIC include/doc_crawler.hpp src/doc_crawler.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/modules/document-store/include>
    PUBLIC $<INSTALL_INTERFACE:include/>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC DocumentStore
)
