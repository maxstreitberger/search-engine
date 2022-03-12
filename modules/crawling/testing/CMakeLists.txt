find_package(Catch2 3 REQUIRED)

add_executable(crawling_tests src/crawler-test-cases.cpp src/document-store-test-cases.cpp)
target_link_libraries(crawling_tests 
    PRIVATE testing
    PRIVATE crawling
    PRIVATE Catch2::Catch2WithMain
)