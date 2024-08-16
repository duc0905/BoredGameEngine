#pragma once

#include "../src/Scene.hpp"
#include "../src/DataAccess/FileManager/Manager.h"
// #include "../src/DataAccess/Actor/ActorManager.hpp"
#include "../src/ECS/Components/IDToPtr.hpp"
#include "../src/ECS/Components/Transform.hpp"
#include "../src/ECS/Components/Camera.hpp"

#if FILE_SYSTEM_API == STDFS_API
#include "../src/DataAccess/FileManager/STDFM.h"
#endif

#if RENDER_API == OGL_API
#include "../src/Adapter/OGL.h"
#include "../src/Frontend/OGLMesh.hpp"
#endif

#include "../src/Frontend/Renderer.hpp"
#include "../src/Frontend/Input.hpp"

#include "../src/Adapter/Window.h"
#include "../src/Adapter/GLFWWindow.h"

#include "../src/GameStruct.hpp"

#include "../src/Utils/DLLLoader/DLLLoader.hpp"

#include "../src/Frontend/RenderUtil.hpp"
