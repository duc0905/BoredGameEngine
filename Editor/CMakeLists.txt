set(SOURCE_FILES
    "src/FileExplorer.cpp"
    "src/app.cpp"
    "src/Adapter/windowAPI.cpp"
)

# add_library(BoredEditor ${SOURCE_FILES})

add_executable(BoredEditor ${SOURCE_FILES})

find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(BoredEditor PRIVATE glfw)
target_link_libraries(BoredEditor PRIVATE imgui::imgui)
target_link_libraries(BoredEditor PRIVATE glad::glad)
target_link_libraries(BoredEditor PRIVATE Engine)

target_include_directories(BoredEditor PRIVATE
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/Engine/include"
          )
