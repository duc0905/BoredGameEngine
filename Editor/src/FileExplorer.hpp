#pragma once
#include "Adapter/api.h"
#include "Adapter/windowAPI.h"
#include "Bored.hpp"
#include "SubWindow.hpp"
#include <functional>

class FileExplorer : public SubWindow
{
public:
  FileExplorer(Manifest& game, int width, int height, Bored::Editor::OSAdapter::Window::WindowAPI& win, Bored::FileSystem::STDFS::Manager& file_manager);
  FileExplorer(Manifest& game, int width, int height);

  virtual void OnSetup() override;
  virtual bool OnUpdate(double dt) override;
  virtual void OnShutdown() override;
  virtual void DrawContent() override;

  void SetOpenFileCallBack(std::function<void(std::shared_ptr<Bored::FileSystem::File>)> callBack);
  void SetOpenProjectCallback(std::function<void(std::string)> cb);

private:
  void OpenWarningWindow();
  void FileDisplayer(std::shared_ptr<Bored::FileSystem::File> file);
  void SubDirDisplayer(std::shared_ptr<Bored::FileSystem::Directory> dir);
  void DirectoryDisplayer();

private:
  bool loadedDir = false;
  bool openContentWindow = false;
  bool errorOpenDir = false;

  Bored::Editor::OSAdapter::OS& win;
  Bored::FileSystem::FileManager& file_manager;
  std::shared_ptr<Bored::FileSystem::Directory> current_dir;

  std::function<void(std::shared_ptr<Bored::FileSystem::File>)> openFileCallBack;
  std::function<void(std::string)> _openProject;
};
