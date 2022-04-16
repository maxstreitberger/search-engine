project(web_crawling_tests)

add_executable(${PROJECT_NAME} main.cpp src/web-crawler-test-cases.cpp src/web-crawler-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE web-crawler
    PRIVATE Catch2::Catch2
    PRIVATE glog::glog
)

create_test(${PROJECT_NAME})