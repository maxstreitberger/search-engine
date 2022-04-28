project(document-store_tests)

add_executable(${PROJECT_NAME} main.cpp src/document-store-test-cases.cpp src/document-store-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE Catch2::Catch2
    PRIVATE DocumentStore
    PRIVATE glog::glog
)

create_test(${PROJECT_NAME})