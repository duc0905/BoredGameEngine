enable_testing()
find_package(GTest)

set(SOURCE_FILES
	main.spec.cpp
)

add_executable(test_main ${SOURCE_FILES})
target_include_directories(test_main PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(test_main GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_main)
