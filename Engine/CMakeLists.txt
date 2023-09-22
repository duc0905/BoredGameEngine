set(SOURCE_FILES
    "src/Bored.cpp"
    "src/Adapter/Audio.cpp"
    "src/Adapter/Render.cpp"
    "src/Adapter/FileSystem.cpp"
    "src/Adapter/Window.cpp"
    "src/Adapter/STDFS.cpp"
    "src/DataAccess/RenderManager.cpp"
    "src/DataAccess/FileManager/FileManager.cpp"
    "src/DataAccess/FileManager/STDFM.cpp"
    )

add_library(Engine ${SOURCE_FILES})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/res")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/Engine")
file(COPY ${RES_SOURCE_DIR} DESTINATION ${RES_DESTINATION_DIR})

configure_file(EngineConfig.h.in ${CMAKE_BINARY_DIR}/EngineConfig.h)

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
