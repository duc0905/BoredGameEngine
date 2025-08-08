set(SOURCE_FILES
    main.cpp
    MyScene.cpp
    Gravity.cpp
)

add_executable(demo2 ${SOURCE_FILES})
target_link_libraries(demo2 PRIVATE Engine)
target_include_directories(demo2 PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
