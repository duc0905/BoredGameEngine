#pragma once

#include "../src/GameLogic.hpp"
#include "../src/DataAccess/FileManager/Manager.h"
// #include "../src/DataAccess/Actor/ActorManager.hpp"

#if FILE_SYSTEM_API == STDFS_API
#include "../src/DataAccess/FileManager/STDFM.h"
#endif

#if RENDER_API == OGL_API
#include "../src/Adapter/OGL.h"
#endif

#include "../src/Frontend/Renderer.hpp"
#include "../src/Frontend/Input.hpp"

#include "../src/Adapter/Window.h"
#include "../src/Adapter/GLFWWindow.h"

#include "../src/GameStruct.hpp"

#include "../src/Utils/DLLLoader/DLLLoader.hpp"
