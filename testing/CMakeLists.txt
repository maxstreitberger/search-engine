add_library(testing INTERFACE)
target_sources(testing INTERFACE include/catch.hpp)
target_include_directories(testing INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_executable(tests src/testing.cpp)
    target_link_libraries(tests testing)