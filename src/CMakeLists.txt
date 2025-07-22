find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(OpenMP)

set(MAIN_SOURCE_FILES
	main.cpp
	Window/Window.cpp
	Renderer/Camera/Camera.cpp
	Renderer/Shader/Shader.cpp
	Renderer/OGL/Renderer.cpp
	# Renderer/CPU/RayTracer.cpp
)

add_executable(main ${MAIN_SOURCE_FILES})
target_link_libraries(main PRIVATE glm::glm)
target_link_libraries(main PRIVATE glad::glad)
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE assimp::assimp)

if (OpenMP_CXX_FOUND)
	target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
endif()
