find_package(Catch2 3 REQUIRED)

add_executable(ranking_testing src/ranker-test-cases.cpp src/e2e.cpp)
target_link_libraries(ranking_testing 
    PRIVATE ranking
    PRIVATE Catch2::Catch2WithMain
)