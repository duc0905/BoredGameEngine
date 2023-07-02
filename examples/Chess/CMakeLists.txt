add_executable(BoredChess "src/main.cpp")

target_link_libraries(BoredChess PRIVATE Engine)
target_include_directories(BoredChess PRIVATE
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_SOURCE_DIR}/Engine/include"
  )