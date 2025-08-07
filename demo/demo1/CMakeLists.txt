set(SOURCE_FILES
    main.cpp
    MyScene.cpp
)

add_executable(demo1 ${SOURCE_FILES})
target_link_libraries(demo1 PRIVATE Engine)
target_include_directories(demo1 PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
