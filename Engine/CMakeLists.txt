# Configurations
set(OGL_API 1)
set(VK_API 2)
set(CPI_API 3)
set(RENDER_API ${OGL_API})

set(STDFS_API 1)
set(VIRTUAL_FS_API 2)
set(FILE_SYSTEM_API ${STDFS_API})

# ============== DEPENDENCIES ================#
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

#=============== MAIN TARGET ================#
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

#=============== OTHER RESOURCES =================#
set(RES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Engine/res")
set(RES_DESTINATION_DIR "${CMAKE_BINARY_DIR}/Engine")
file(COPY ${RES_SOURCE_DIR} DESTINATION ${RES_DESTINATION_DIR})

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)

target_link_libraries(Engine PRIVATE OpenAL::OpenAL)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PRIVATE assimp::assimp)
target_include_directories(Engine PRIVATE "${CMAKE_BINARY_DIR}")

#============== INSTALLING ===============#
install(TARGETS Engine COMPONENT Engine)
install(FILES "src/Scene.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Engine"
    COMPONENT Engine)
install(
    DIRECTORY "src/Adapter" "src/DataAccess" "src/ECS" "src/Frontend" "src/Utils"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Engine"
    COMPONENT Engine
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp")

#=============== TESTING ================#
set(ENGINE_TEST_FILES
    "src/Frontend/Factory.spec.cpp"
    "src/Frontend/Mesh.spec.cpp"
    "src/Frontend/Texture.spec.cpp"
)

add_executable(EngineTest ${ENGINE_TEST_FILES})
target_link_libraries(EngineTest Engine)
target_link_libraries(EngineTest GTest::gtest_main GTest::gmock_main)

target_include_directories(EngineTest PRIVATE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/Engine")

include(GoogleTest)
gtest_discover_tests(EngineTest TEST_PREFIX "BoredEngine::")

add_custom_target(InstallEngine
    COMMAND ${CMAKE_COMMAND} cmake -DCOMPONENT=Engine -P ${CMAKE_BINARY_DIR}/cmake_install.cmake)
