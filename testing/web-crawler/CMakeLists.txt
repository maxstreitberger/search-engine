project(web-crawler_tests)

add_executable(${PROJECT_NAME} main.cpp src/web-crawler-test-cases.cpp src/web-crawler-end-to-end-test.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE WebCrawler
    PRIVATE Catch2::Catch2
    PRIVATE glog::glog
)

create_test(${PROJECT_NAME})