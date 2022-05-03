project(WebCrawler)

add_library(${PROJECT_NAME} STATIC include/web_crawler.hpp src/web_crawler.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include/>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glog::glog
    PUBLIC unofficial::gumbo::gumbo
    PUBLIC ${CURL_LIBRARIES}
    PUBLIC DocumentStore
)
