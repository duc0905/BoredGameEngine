add_executable(BoredChess "src/main.cpp")

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/examples/Chess/resource")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/examples/Chess/resource")

set(WAV_FILE "${RES_SOURCE_DIR}/example.wav")

configure_file(${WAV_FILE} ${RES_DESTINATION_DIR}/example.wav COPYONLY)

find_package(OpenAL REQUIRED)
find_package(AudioFile REQUIRED)

target_link_libraries(BoredChess PUBLIC AudioFile)
target_link_libraries(BoredChess PUBLIC OpenAL::OpenAL)

target_link_libraries(BoredChess PUBLIC Engine)
target_include_directories(BoredChess PUBLIC
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_SOURCE_DIR}/Engine/include"
  )