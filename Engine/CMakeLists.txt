# Configurations
set(OGL_API 1)
set(VK_API 2)
set(CPI_API 3)
set(RENDER_API ${OGL_API})

set(STDFS_API 1)
set(VIRTUAL_FS_API 2)
set(FILE_SYSTEM_API ${STDFS_API})

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Main target
set(ENGINE_SOURCE_FILES
    "src/Adapter/Audio.cpp"
    "src/Adapter/Render.cpp"
    "src/Adapter/FileSystem.cpp"
    "src/Adapter/GLFWWindow.cpp"
    "src/DataAccess/FileManager/Manager.cpp"
    "src/ECS/ActorManager.cpp"
    "src/Frontend/Renderer.cpp"
    "src/Frontend/RenderUtil.cpp"
    "src/Frontend/Input.cpp"
    "src/Scene.cpp"
    "src/Bored.cpp"
    )

if (${RENDER_API} EQUAL ${OGL_API})
    list(APPEND ENGINE_SOURCE_FILES "src/Adapter/OGL.cpp")
    list(APPEND ENGINE_SOURCE_FILES "src/Frontend/OGL/Mesh.cpp")
    list(APPEND ENGINE_SOURCE_FILES "src/Frontend/OGL/Texture.cpp")
    list(APPEND ENGINE_SOURCE_FILES "src/Frontend/OGL/Factory.cpp")
endif()

add_library(Engine ${ENGINE_SOURCE_FILES})

configure_file(EngineConfig.h.in ${CMAKE_BINARY_DIR}/EngineConfig.h)

set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/res")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/Engine")
file(COPY ${RES_SOURCE_DIR} DESTINATION ${RES_DESTINATION_DIR})

set(INCLUDE_DESTINATION_DIR "${CMAKE_BINARY_DIR}/include/Engine")
file(COPY "src/Scene.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}")
file(COPY "src/GameStruct.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}")
file(COPY "src/DataAccess/FileManager/Manager.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/DataAccess/FileManager")
file(COPY "src/ECS/Components/IDToPtr.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/ECS/Components")
file(COPY "src/ECS/Components/Transform.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/ECS/Components")
file(COPY "src/ECS/Components/Camera.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/ECS/Components")
file(COPY "src/ECS/Actor.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/ECS")
file(COPY "src/ECS/ActorManager.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/ECS")
file(COPY "src/Frontend/RenderUtil.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend")
file(COPY "src/Frontend/Renderer.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend")
file(COPY "src/Frontend/Input.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend")
file(COPY "src/Adapter/Window.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/Adapter")
file(COPY "src/Adapter/Render.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/Adapter")
file(COPY "src/Adapter/FileSystem.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/Adapter")
file(COPY "src/Adapter/GLFWWindow.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/Adapter")
file(COPY "src/Utils/DLLLoader/DLLLoader.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Utils/DLLLoader")

if (${FILE_SYSTEM_API} EQUAL ${STDFS_API})
    file(COPY "src/DataAccess/FileManager/STDFM.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/DataAccess/FileManager")
endif()


file(COPY "src/Frontend/CPU/Mesh.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/CPU")
file(COPY "src/Frontend/CPU/Texture.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/CPU")
file(COPY "src/Frontend/CPU/Factory.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/CPU")
if(${RENDER_API} EQUAL ${OGL_API})
    file(COPY "src/Adapter/OGL.h" DESTINATION "${INCLUDE_DESTINATION_DIR}/Adapter")
    file(COPY "src/Frontend/OGL/Mesh.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/OGL")
    file(COPY "src/Frontend/OGL/Texture.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/OGL")
    file(COPY "src/Frontend/OGL/Factory.hpp" DESTINATION "${INCLUDE_DESTINATION_DIR}/Frontend/OGL")
endif()

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_link_libraries(Engine PRIVATE OpenAL::OpenAL)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PRIVATE assimp::assimp)
target_include_directories(Engine PRIVATE "${CMAKE_BINARY_DIR}")

set(ENGINE_TEST_FILES
    "src/Frontend/Factory.spec.cpp"
    "src/Frontend/Mesh.spec.cpp"
    "src/Frontend/Texture.spec.cpp"
)

add_executable(EngineTest ${ENGINE_TEST_FILES})
target_link_libraries(EngineTest Engine)
target_link_libraries(EngineTest GTest::gtest_main GTest::gmock_main)

target_include_directories(EngineTest PRIVATE ${INCLUDE_DESTINATION_DIR})
target_include_directories(EngineTest PRIVATE ${CMAKE_BINARY_DIR})

include(GoogleTest)
gtest_discover_tests(EngineTest)
