project(document_crawling_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} src/document-crawler-test-cases.cpp src/e2e-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE document-crawler
    PRIVATE doc_store
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})