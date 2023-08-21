add_library(Engine "src/Bored.cpp" "src/Adapter/Audio.cpp")

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/examples/Chess/resource")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/examples/Chess/resource")

set(WAV_FILE "${RES_SOURCE_DIR}/sound.wav")

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

configure_file(${WAV_FILE} ${RES_DESTINATION_DIR}/sound.wav COPYONLY)

find_package(OpenAL CONFIG REQUIRED)
find_package(AudioFile CONFIG REQUIRED)

target_link_libraries(Engine PUBLIC OpenAL::OpenAL)
target_link_libraries(Engine PUBLIC AudioFile)

target_include_directories(Engine PRIVATE
          "${CMAKE_BINARY_DIR}"
          "include" # Engine's include directory
          )
