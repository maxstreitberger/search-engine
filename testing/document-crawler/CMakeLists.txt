project(document_crawling_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp src/document-crawler-test-cases.cpp src/document-crawler-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE document-crawler
    PRIVATE doc_store
    PRIVATE Catch2::Catch2
)

create_test(${PROJECT_NAME})