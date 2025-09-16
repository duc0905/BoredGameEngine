set(SOURCE_FILES
    src/main.cpp
    src/MazeScene.cpp
)

add_executable(maze ${SOURCE_FILES})
target_link_libraries(maze PRIVATE Engine)
target_include_directories(maze PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
