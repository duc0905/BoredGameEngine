add_library(Engine "src/Bored.cpp" "src/Adapter/Audio.cpp")

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/resource")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/Engine/resource")

set(WAV_FILE "${RES_SOURCE_DIR}/example.wav")

configure_file(${WAV_FILE} ${RES_DESTINATION_DIR}/example.wav COPYONLY)

find_package(OpenAL REQUIRED)
find_package(AudioFile REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(Engine PRIVATE OpenAL::OpenAL)
target_link_libraries(Engine PRIVATE glm::glm)
target_link_libraries(Engine PRIVATE AudioFile)

target_include_directories(Engine PRIVATE
          "${CMAKE_BINARY_DIR}"
          "include" # Engine's include directory
          )
