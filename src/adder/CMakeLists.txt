project (adder)

add_library(${PROJECT_NAME} src/adder.cpp)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)