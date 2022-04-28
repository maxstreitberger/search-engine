project(document-crawler_tests)

add_executable(${PROJECT_NAME} main.cpp src/document-crawler-test-cases.cpp src/document-crawler-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE DocumentCrawler
    PRIVATE Catch2::Catch2
    PRIVATE glog::glog
)

create_test(${PROJECT_NAME})