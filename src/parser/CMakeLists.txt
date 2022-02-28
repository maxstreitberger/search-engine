project (parser)

add_library(${PROJECT_NAME} src/parser.cpp)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/documents
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})