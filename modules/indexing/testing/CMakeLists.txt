find_package(Catch2 3 REQUIRED)

add_executable(indexing_tests src/indexer-test-cases.cpp)
target_link_libraries(indexing_tests 
    PRIVATE indexing
    PRIVATE Catch2::Catch2WithMain
)