configure_file(PlaygroundConfig.h.in ${CMAKE_BINARY_DIR}/PlaygroundConfig.h)

set(SOURCE_FILES
	"src/main.cpp"
)
add_executable(Playground ${SOURCE_FILES})
target_link_libraries(Playground PRIVATE Engine)
target_include_directories(Playground PRIVATE
					"${CMAKE_BINARY_DIR}"
					"${CMAKE_SOURCE_DIR}/Engine/include"
					"${CMAKE_SOURCE_DIR}/Engine/src" # Ultimate power unlocked
)

add_executable(MyTest src/tests/test.cpp)
target_link_libraries(MyTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(MyTest)
