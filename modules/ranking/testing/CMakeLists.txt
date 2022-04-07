project(ranking_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/ranker-test-cases.cpp src/ranker-end-to-end-test.cpp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE ranking
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})