project(web-crawler)

add_library(${PROJECT_NAME} src/web_crawler.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${CURL_INCLUDE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE glog::glog
    PUBLIC ${CURL_LIBRARIES}
    PUBLIC doc_store
)
