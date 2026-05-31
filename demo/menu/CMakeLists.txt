set(SOURCE_FILES
    main.cpp
    Scene2D.cpp
)

add_executable(menu ${SOURCE_FILES})
target_link_libraries(menu PRIVATE Engine)
target_include_directories(menu PRIVATE "${PROJECT_SOURCE_DIR}/Engine")
