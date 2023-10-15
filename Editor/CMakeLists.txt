# Targets configs

# Targets
set(SOURCE_FILES
    "src/FileExplorer.cpp"
    "src/FileContent.cpp"
    "src/GameScreen.cpp"
    "src/Adapter/windowAPI.cpp"
)

add_library(Stuff ${SOURCE_FILES})
add_executable(BoredEditor "src/app.cpp")

find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(BoredEditor PRIVATE glfw)
target_link_libraries(BoredEditor PRIVATE imgui::imgui)
target_link_libraries(BoredEditor PRIVATE glad::glad)

target_link_libraries(Stuff PRIVATE glfw)
target_link_libraries(Stuff PRIVATE imgui::imgui)
target_link_libraries(Stuff PRIVATE glad::glad)
target_link_libraries(Stuff PRIVATE Engine)
target_link_libraries(BoredEditor PRIVATE Stuff)

target_include_directories(BoredEditor PRIVATE
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/Engine/include"
)

target_include_directories(Stuff PRIVATE
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/Engine/include"
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
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/examples/Playground/src"
)

include(GoogleTest)
gtest_discover_tests(EditorTest)
