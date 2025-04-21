set(MAIN_SOURCE_FILES
	main.cpp
	Camera/Camera.cpp
)

add_executable(main ${MAIN_SOURCE_FILES})

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)
