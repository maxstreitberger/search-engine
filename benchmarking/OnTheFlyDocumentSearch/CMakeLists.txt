project(OnTheFlyDocumentSearch_benchmarking)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/benchmarking/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE document-crawler 
    PRIVATE indexing
    PRIVATE ranking
    PRIVATE glog::glog
)
