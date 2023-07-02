add_executable(BoredEditor src/app.cpp)

target_link_libraries(BoredEditor PRIVATE Engine)
target_include_directories(BoredEditor PRIVATE
          "${CMAKE_BINARY_DIR}"
          "${CMAKE_SOURCE_DIR}/Engine/include"
          )