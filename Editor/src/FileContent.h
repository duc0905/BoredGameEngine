#pragma once
#include "Bored.hpp"
#include "window.h"
#include <memory>

namespace Bored {
namespace Editor {
class FileContentWindow : public BoredWindow {
 public:
  FileContentWindow(int width, int height,
                    std::shared_ptr<FileSystem::File> file);

  // FileContentWindow(int width, int height);

  virtual void Init() override;
  virtual void Update() override;
  virtual void Render() override;
  virtual void Shutdown() override;

  virtual bool IsRunning() const override;

  void SetFileToDisplay(std::shared_ptr<FileSystem::File> file);

 private:
  std::shared_ptr<FileSystem::File> current_file;
  std::string ProcessFileNameToTitle(std::shared_ptr<FileSystem::File> file);

 private:
  //   FileContentWindow(std::shared_ptr<FileSystem::File> file);
};
}  // namespace Editor
}  // namespace Bored
