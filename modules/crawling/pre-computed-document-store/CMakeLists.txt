project (PreComputedDocumentStore)

add_library(${PROJECT_NAME} src/pre_computed_doc_store.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
