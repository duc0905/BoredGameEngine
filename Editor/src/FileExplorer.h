#pragma once
#include "Adapter/api.h"
#include "Adapter/windowAPI.h"
#include "Bored.hpp"
#include "window.h"
#include <functional>
#include <string>

namespace Bored
{
namespace Editor
{
class FileExplorer : public Window
{
  public:
    FileExplorer(int width, int height, OSAdapter::Window::WindowAPI& win, FileSystem::STDFS::Manager& file_manager);
    FileExplorer(int width, int height);

    virtual void Init() override;
    virtual void Update() override;
    virtual void Shutdown() override;

    void SetOpenFileCallBack(std::function<void(std::shared_ptr<FileSystem::File>)> callBack);

  private:
    void OpenWarningWindow();
    void FileDisplayer(std::shared_ptr<Bored::FileSystem::File> file);
    void SubDirDisplayer(std::shared_ptr<Bored::FileSystem::Directory> dir);
    void DirectoryDisplayer();

  private:
    bool loadedDir = false;
    bool openContentWindow = false;
    bool errorOpenDir = false;
    OSAdapter::OS& win;
    FileSystem::FileManager& file_manager;
    std::shared_ptr<FileSystem::Directory> current_dir;
    std::function<void(std::shared_ptr<FileSystem::File>)> openFileCallBack;
};
} // namespace Editor
} // namespace Bored
