project(helper_tests)

find_package(Catch2 3 REQUIRED)

add_executable(${PROJECT_NAME} test-cases.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Catch2::Catch2WithMain
)

create_test(${PROJECT_NAME})