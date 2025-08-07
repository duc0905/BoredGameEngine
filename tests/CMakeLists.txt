enable_testing()
find_package(GTest)
find_package(glad)
find_package(glfw3)

set(SOURCE_FILES
	main.spec.cpp
	Texture.spec.cpp
)

add_executable(test_main ${SOURCE_FILES})
target_include_directories(test_main PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
target_link_libraries(test_main PRIVATE GTest::gtest_main)
target_link_libraries(test_main PRIVATE glfw)
target_link_libraries(test_main PRIVATE glad::glad)

include(GoogleTest)
gtest_discover_tests(test_main)
