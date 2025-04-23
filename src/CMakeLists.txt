set(MAIN_SOURCE_FILES
	main.cpp
	Window/Window.cpp
	Renderer/Camera/Camera.cpp
	Renderer/Shader/Shader.cpp
)

add_executable(main ${MAIN_SOURCE_FILES})

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)
target_link_libraries(main PRIVATE glad::glad)
target_link_libraries(main PRIVATE glfw)
