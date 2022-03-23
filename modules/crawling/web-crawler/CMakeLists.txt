project (web-crawler)

set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

find_package(CURL REQUIRED) 

add_library(${PROJECT_NAME} src/crawler.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CURL_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${CURL_LIBRARIES}
)

add_subdirectory(testing)