project(indexing_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/indexer-test-cases.cpp src/indexer-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE indexing
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})