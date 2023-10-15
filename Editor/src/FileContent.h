#pragma once
#include "Bored.hpp"
#include "SubWindow.h"
#include <memory>

namespace Bored
{
namespace Editor
{
class FileContentWindow : public SubWindow
{
  public:
    FileContentWindow(int width, int height, std::shared_ptr<FileSystem::File> file);

    virtual void OnInit() override;
    virtual void OnUpdate() override;
    virtual void OnShutdown() override;

    void SetFileToDisplay(std::shared_ptr<FileSystem::File> file);

  private:
    std::shared_ptr<FileSystem::File> current_file;
    std::string ProcessFileNameToTitle(std::shared_ptr<FileSystem::File> file);

  private:
    //   FileContentWindow(std::shared_ptr<FileSystem::File> file);
};
} // namespace Editor
} // namespace Bored
