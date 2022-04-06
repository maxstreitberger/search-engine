project(document_store_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} src/document-store-test-cases.cpp src/document-store-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE Catch2::Catch2WithMain
    PRIVATE doc_store
)

create_test(${PROJECT_NAME})