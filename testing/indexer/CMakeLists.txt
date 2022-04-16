project(indexing_tests)

add_executable(${PROJECT_NAME} main.cpp src/indexer-test-cases.cpp src/indexer-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE indexing
    PRIVATE glog::glog
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})