set(SOURCE_FILES
    "src/Bored.cpp"
    "src/Adapter/Audio.cpp"
    "src/Adapter/OAL.cpp"
    "src/Adapter/Render.cpp"
    "src/Adapter/OGL.cpp"
    "src/Adapter/FileSystem.cpp"
    "src/Adapter/STDFS.cpp"
    )

add_library(Engine ${SOURCE_FILES})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/examples/Chess/resource")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/examples/Chess/resource")

set(WAV_FILE "${RES_SOURCE_DIR}/sound.wav")
configure_file(${WAV_FILE} ${RES_DESTINATION_DIR}/sound.wav COPYONLY)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

target_link_libraries(Engine PRIVATE OpenAL::OpenAL)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)

target_include_directories(Engine PRIVATE
          "${CMAKE_BINARY_DIR}"
          "include" # Engine's include directory
          )
