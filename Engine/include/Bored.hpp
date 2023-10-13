#pragma once

#include "../src/DataAccess/FileManager/Manager.h"
#include "../src/GameLoop.hpp"

#if FILE_SYSTEM_API == STDFS_API
#include "../src/DataAccess/FileManager/STDFM.h"
#endif

#include "../src/Frontend/Renderer.hpp"
