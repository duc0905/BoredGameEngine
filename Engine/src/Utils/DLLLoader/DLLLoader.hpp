#pragma once
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

#include <string>
#include <memory>
#include <stdexcept>

#define CREATE_INSTANCE "CreateInstance"
#define DELETE_INSTANCE "DeleteInstance"

namespace Bored {
namespace Util {

/** TODO: Write this
    * TODO: Move implementations to cpp file
    */
class DLLLoader {

private:
#if defined _WIN32 || defined _WIN64
    HMODULE m_handle = nullptr;
#elif defined __linux__
    void* m_handle = nullptr;
#endif

public:
    DLLLoader() = default;
    DLLLoader(const DLLLoader&) = delete;
    DLLLoader(DLLLoader&&) = delete;
    DLLLoader& operator=(const DLLLoader&) = delete;
    DLLLoader& operator=(DLLLoader&&) = delete;
    ~DLLLoader() = default;

    void Load(const std::string& path) {
#if defined _WIN32 || defined _WIN64
        m_handle = LoadLibrary(path.c_str());
#elif defined __linux__
        m_handle = dlopen(path.c_str(), RTLD_LAZY);
#endif

        if (!m_handle) {
            throw std::runtime_error("Failed to load DLL: " + path);
        }
    }

    void Unload() {
        if (m_handle) {
#if defined _WIN32 || defined _WIN64
            FreeLibrary(m_handle);
#elif __linux__
            dlclose(m_handle);
#endif
            m_handle = nullptr;
        }
    }

    template<typename T>
    T GetFunction(const std::string& name) {
        if (!m_handle) {
            throw std::runtime_error("DLL not loaded");
        }

#if defined _WIN32 || defined _WIN64
        auto func = GetProcAddress(m_handle, name.c_str());
#elif __linux__
        auto func = dlsym(m_handle, name.c_str());
#endif
        if (!func) {
            throw std::runtime_error("Failed to get function: " + name);
        }

        return reinterpret_cast<T>(func);
    }

    template<typename T>
    std::shared_ptr<T> GetIntance() {
        if (!m_handle) {
            throw std::runtime_error("DLL not loaded");
        }

#if defined _WIN32 || defined _WIN64
        auto func = GetProcAddress(m_handle, CREATE_INSTANCE);
#elif __linux__
        auto func = dlsym(m_handle, CREATE_INSTANCE);
#endif
        if (!func) {
            throw std::runtime_error(std::string("Failed to get function: ") + CREATE_INSTANCE);
        }

#if defined _WIN32 || defined _WIN64
        auto del_func = GetProcAddress(m_handle, DELETE_INSTANCE);
#elif __linux__
        auto del_func = dlsym(m_handle, DELETE_INSTANCE);
#endif
        if (!del_func) {
            throw std::runtime_error(std::string("Failed to get function: ") + DELETE_INSTANCE);
        }

        auto createInstance = reinterpret_cast<T* (*)()>(func);
        auto deleteInstance = reinterpret_cast<void(*)(T*)>(del_func);
        return std::shared_ptr<T>(createInstance());
    }
};

}
}
