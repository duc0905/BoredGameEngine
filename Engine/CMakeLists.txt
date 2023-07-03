add_library(Engine "src/Bored.cpp" "src/Adapter/Audio.cpp")


find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(Engine PRIVATE OpenAL::OpenAL)

target_include_directories(Engine PRIVATE
          "${CMAKE_BINARY_DIR}"
          "include" # Engine's include directory
          )
