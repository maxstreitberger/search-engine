project (ConcurrentDocumentStore)

add_library(${PROJECT_NAME} src/concurrent_doc_store.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
