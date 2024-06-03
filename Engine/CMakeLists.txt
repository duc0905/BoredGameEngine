# Configurations
set(OGL_API 1)
set(VK_API 2)
set(CPI_API 3)
set(RENDER_API ${OGL_API})

set(STDFS_API 1)
set(VIRTUAL_FS_API 2)
set(FILE_SYSTEM_API ${STDFS_API})

configure_file(EngineConfig.h.in ${CMAKE_BINARY_DIR}/EngineConfig.h)

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/res")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/Engine")
file(COPY ${RES_SOURCE_DIR} DESTINATION ${RES_DESTINATION_DIR})

# Main target
set(SOURCE_FILES
    "src/Adapter/Audio.cpp"
    "src/Adapter/Render.cpp"
    "src/Adapter/FileSystem.cpp"
    "src/Adapter/GLFWWindow.cpp"
    "src/DataAccess/Render/Manager.cpp"
    "src/DataAccess/FileManager/Manager.cpp"
    "src/Frontend/Renderer.cpp"
    "src/Frontend/Input.cpp"
    "src/GameLogic.cpp"
    "src/Bored.cpp"
    )

if (${RENDER_API} EQUAL ${OGL_API})
list(APPEND SOURCE_FILES "src/Adapter/OGL.cpp")
endif()

add_library(Engine ${SOURCE_FILES})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

target_link_libraries(Engine PUBLIC OpenAL::OpenAL)
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)

target_include_directories(Engine PRIVATE "${CMAKE_BINARY_DIR}")
