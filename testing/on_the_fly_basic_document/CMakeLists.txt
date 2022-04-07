project(on_the_fly_basic_document_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp test-cases.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE document-crawler 
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})