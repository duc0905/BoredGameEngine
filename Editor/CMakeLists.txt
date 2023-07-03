add_executable(BoredEditor src/app.cpp)

find_package(imgui REQUIRED)
find_package(glad REQUIRED)

target_link_libraries(BoredEditor PRIVATE Engine)
target_link_libraries(BoredEditor PRIVATE imgui::imgui)
target_link_libraries(BoredEditor PRIVATE glad::glad)
target_include_directories(BoredEditor PRIVATE
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/Engine/include"
          )