add_library(Engine "src/Bored.cpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

find_package(Stb REQUIRED)
find_package(assimp CONFIG REQUIRED)

find_package(AudioFile CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries (Engine 
          PRIVATE glfw 
          PRIVATE glad::glad
          PRIVATE ${Stb_INCLUDE_DIR}
          PRIVATE assimp::assimp
          PRIVATE AudioFile
          PRIVATE EnTT::EnTT
          PRIVATE glm::glm
          )

target_include_directories(Engine PRIVATE 
          "${CMAKE_BINARY_DIR}"
          "include" # Engine's include directory
          )