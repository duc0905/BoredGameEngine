find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

set(EDITOR_SOURCE_FILES main.cpp MyScene.cpp)

add_executable(Editor ${EDITOR_SOURCE_FILES})
target_link_libraries(Editor PRIVATE Engine)
target_link_libraries(Editor PRIVATE imgui::imgui)
target_link_libraries(Editor PRIVATE EnTT::EnTT)
target_include_directories(Editor PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
