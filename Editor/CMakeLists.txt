# Targets configs

# Targets
set(SOURCE_FILES
    "src/Editor.cpp"
    "src/SubWindow/SubWindow.cpp"
    "src/SubWindow/FileExplorer.cpp"
    "src/SubWindow/FileContent.cpp"
    "src/SubWindow/GameScreen.cpp"
    "src/Adapter/windowAPI.cpp"
    "src/GameManifest/Loader.cpp"
)

add_library(Stuff ${SOURCE_FILES})

find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)

target_link_libraries(Stuff PRIVATE glfw)
target_link_libraries(Stuff PRIVATE imgui::imgui)
target_link_libraries(Stuff PRIVATE glad::glad)
target_link_libraries(Stuff PRIVATE Engine)
target_link_libraries(Stuff PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(Stuff PRIVATE
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}/include"
)

add_executable(Editor "src/app.cpp")
target_link_libraries(Editor PRIVATE imgui::imgui)
target_link_libraries(Editor PRIVATE tinyfiledialogs::tinyfiledialogs)
target_link_libraries(Editor PRIVATE Stuff)
target_include_directories(Editor PRIVATE
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_BINARY_DIR}/include"
)

# Testing
set(TEST_FILES
    "tests/FileExplorer.cpp"
    "tests/GameScreen.cpp"
)

add_executable(EditorTest ${TEST_FILES})

target_link_libraries(EditorTest Stuff)
target_link_libraries(EditorTest GTest::gtest_main)

target_include_directories(EditorTest PRIVATE
    "${CMAKE_BINARY_DIR}/include"
    "${CMAKE_SOURCE_DIR}/Editor/src"
)

include(GoogleTest)
gtest_discover_tests(EditorTest)
