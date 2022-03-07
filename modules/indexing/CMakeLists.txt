project (indexing)

add_library(${PROJECT_NAME} src/parser.cpp)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(testing)