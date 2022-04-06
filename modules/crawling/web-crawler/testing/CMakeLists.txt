project(web_crawling_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} src/web-crawler-test-cases.cpp src/web-crawler-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE web-crawler
    PRIVATE Catch2::Catch2WithMain
    PRIVATE doc_store
)

create_test(${PROJECT_NAME})