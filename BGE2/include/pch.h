// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
// std
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>
#include <set>
#include <array>
#include <string>
#include <fstream>
#include <typeindex>
#include <typeinfo>

// imgui
#include <imgui.h>

// entt
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

// glm
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

// assimp & stb
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <stb_image.h>

#endif //PCH_H
