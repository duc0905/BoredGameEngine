find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(OpenMP)

set(MAIN_SOURCE_FILES
	main.cpp
	Window/Window.cpp
	Systems/Renderer/Shader/Shader.cpp
	Systems/Renderer/OGL/Renderer.cpp
	Components/Mesh3D.cpp
	# Renderer/CPU/RayTracer.cpp
)

add_executable(main ${MAIN_SOURCE_FILES})
target_link_libraries(main PRIVATE glm::glm)
target_link_libraries(main PRIVATE glad::glad)
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE assimp::assimp)
target_link_libraries(main PRIVATE EnTT::EnTT)

if (OpenMP_CXX_FOUND)
	target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
endif()
