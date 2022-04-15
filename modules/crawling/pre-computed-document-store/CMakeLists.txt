project (pre_computed_doc_store)

add_library(${PROJECT_NAME} src/pre_computed_doc_store.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC glog::glog
)
