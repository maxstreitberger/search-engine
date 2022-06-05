project(ConcurrentOnTheFlyDocumentSearch_benchmarking)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/benchmarking/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ConcurrentDocumentCrawler
    PRIVATE ConcurrentDocumentStore
    PRIVATE ConcurrentIndexer
    PRIVATE Ranker
)
