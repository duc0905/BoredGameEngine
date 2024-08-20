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
    # "src/DataAccess/Render/Manager.cpp"
    "src/DataAccess/FileManager/Manager.cpp"
    "src/ECS/ActorManager.cpp"
    "src/Frontend/Renderer.cpp"
    "src/Frontend/RenderUtil.cpp"
    "src/Frontend/Input.cpp"
    # "src/Frontend/RenderUtil.cpp"
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

set(INCLUDE_FILES "src/ECS/ActorManager.hpp")
set(INCLUDE_DESTINATION_DIR "${CMAKE_BINARY_DIR}/include")
file(COPY ${INCLUDE_FILES} DESTINATION ${INCLUDE_DESTINATION_DIR})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_link_libraries(Engine PRIVATE OpenAL::OpenAL)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PRIVATE assimp::assimp)
target_include_directories(Engine PRIVATE "${CMAKE_BINARY_DIR}")
#
# set(ACTOR_MNG_FILES
#     "src/ECS/ActorManager.cpp"
# )
#
# add_library(ActorManager ${ACTOR_MNG_FILES})
# target_link_libraries(ActorManager PRIVATE EnTT::EnTT)
#
# install(TARGETS ActorManager)
# install(FILES "src/DataAccess/Actor/ActorManager.hpp" DESTINATION "include/Actor")
# # target_compile_definitions(ActorManager PRIVATE ACTORMANAGER_EXPORTS)

set(ENGINE_TEST_FILES
    "src/Frontend/CPU/Factory.spec.cpp"
    "src/Frontend/CPU/Mesh.spec.cpp"
    "src/Frontend/CPU/Texture.spec.cpp"
)

add_executable(EngineTest ${ENGINE_TEST_FILES})
target_link_libraries(EngineTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(EngineTest)
