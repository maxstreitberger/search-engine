project(ranker_tests)

add_executable(${PROJECT_NAME} main.cpp src/ranker-test-cases.cpp src/ranker-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE ranking
    PRIVATE glog::glog
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})