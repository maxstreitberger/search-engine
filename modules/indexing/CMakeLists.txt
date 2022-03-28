project (indexing)

add_library(${PROJECT_NAME} src/indexer.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC glog::glog
    PUBLIC nlohmann_json::nlohmann_json
)

add_subdirectory(testing)