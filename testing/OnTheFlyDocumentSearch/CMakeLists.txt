project(OnTheFlyDocumentSearch_tests)

add_executable(${PROJECT_NAME} main.cpp test-cases.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DocumentCrawler 
    PRIVATE Indexer
    PRIVATE Ranker
    PRIVATE Catch2::Catch2WithMain
    PRIVATE glog::glog
)

create_test(${PROJECT_NAME})