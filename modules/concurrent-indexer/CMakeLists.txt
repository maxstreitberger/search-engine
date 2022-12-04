project(ConcurrentIndexer)

add_library(${PROJECT_NAME} src/concurrent_indexer.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
