set(SOURCE_FILES
    src/main.cpp
)

add_executable(network ${SOURCE_FILES})
target_link_libraries(network PRIVATE Engine)
target_include_directories(network PRIVATE "${PROJECT_SOURCE_DIR}/Engine") 
