project(document-crawler)

add_library(${PROJECT_NAME} src/doc_crawler.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glog::glog
    PUBLIC doc_store
)
