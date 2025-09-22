find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(OpenMP)

set(ENGINE_SOURCE_FILES
	Systems/Input/IOService.cpp
	Systems/Renderer/Shader/Shader.cpp
	Systems/Renderer/OGL/Renderer.cpp
	Components/Mesh3D.cpp
	# Renderer/CPU/RayTracer.cpp
	Utils/AssetManager.cpp
)

add_library(Engine ${ENGINE_SOURCE_FILES})
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PRIVATE EnTT::EnTT)
target_link_libraries(Engine PRIVATE glad::glad)
target_link_libraries(Engine PRIVATE glfw)
target_link_libraries(Engine PRIVATE assimp::assimp)
target_include_directories(Engine PRIVATE ${Stb_INCLUDE_DIR})

if (OpenMP_CXX_FOUND)
	target_link_libraries(Engine PRIVATE OpenMP::OpenMP_CXX)
endif()
