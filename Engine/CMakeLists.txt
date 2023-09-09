set(SOURCE_FILES
    "src/Bored.cpp"
    "src/Adapter/Audio.cpp"
    "src/Adapter/OAL.cpp"
    "src/Adapter/Render.cpp"
    "src/Adapter/OGL.cpp"
    "src/Adapter/FileSystem.cpp"
     "src/Adapter/Window.cpp"
    "src/Adapter/STDFS.cpp"
    )

add_library(Engine ${SOURCE_FILES})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

configure_file(src/config.h.in src/config.h)

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
