#include "EngineConfig.h"

// #if RENDER_API == OGL_API
// #include "Adapter/OGL.cpp"
// #endif

#if FILE_SYSTEM_API == STDFS_API
#include "DataAccess/FileManager/STDFM.cpp"
#endif