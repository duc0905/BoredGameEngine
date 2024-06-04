add_library(ChessLogic SHARED
  "src/ChessLogic.cpp")

target_link_libraries(ChessLogic PRIVATE Engine)

target_include_directories(ChessLogic PRIVATE
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_SOURCE_DIR}/Engine/include"
  )
