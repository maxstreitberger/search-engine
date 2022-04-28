project(DocumentStore)

add_library(${PROJECT_NAME} STATIC include/doc_store.hpp src/doc_store.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include/>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glog::glog
)
