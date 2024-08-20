add_library(Chess SHARED
  "src/ChessLogic.cpp")

target_link_libraries(Chess PRIVATE Engine)

target_include_directories(Chess PRIVATE
  "${CMAKE_BINARY_DIR}/include"
)

add_executable(BoredChess
    "src/main.cpp"
    "src/ChessLogic.cpp")


target_link_libraries(BoredChess PRIVATE Engine)

target_include_directories(BoredChess PRIVATE
  "${CMAKE_BINARY_DIR}/include"
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}"
)

set(MAN_FILE "./Bored.json")
file(COPY ${MAN_FILE} DESTINATION "${CMAKE_BINARY_DIR}/examples/Chess")

set(RES_SOURCE_DIR "./res")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/examples/Chess")
file(COPY ${RES_SOURCE_DIR} DESTINATION ${RES_DESTINATION_DIR})
