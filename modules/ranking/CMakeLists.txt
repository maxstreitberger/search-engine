project (ranking)

set(CMAKE_PREFIX_PATH "~/vcpkg/installed/x64-osx/share")

find_package(nlohmann_json CONFIG REQUIRED)

add_library(${PROJECT_NAME} src/ranker.cpp)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

# add_subdirectory(testing)