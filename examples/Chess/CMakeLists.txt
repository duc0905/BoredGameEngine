add_library(Chess SHARED
  "src/ChessLogic.cpp")

target_link_libraries(Chess PRIVATE Engine)

target_include_directories(Chess PRIVATE
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_SOURCE_DIR}/Engine/include"
)

add_executable(BoredChess
  "src/main.cpp")

target_link_libraries(BoredChess PRIVATE Engine)

target_include_directories(BoredChess PRIVATE
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_SOURCE_DIR}/Engine/include"
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}"
)

set(MAN_FILE "./Bored.json")
file(COPY ${MAN_FILE} DESTINATION "${CMAKE_BINARY_DIR}/examples/Chess")
