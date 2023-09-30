#pragma once
#include "../../Adapter/STDFS.h"
#include "FileManager.h"
#include <memory>

namespace Bored {
namespace FileSystem {
namespace STDFS {
class Manager : public FileManager {
 public:
  static Manager& GetInstance();
  virtual std::shared_ptr<FileSystem::File> CreateFile(
      const std::string& filePath) override;

  virtual std::shared_ptr<FileSystem::Directory> MakeDirectory(
      const std::string& dirPath) override;

 private:
  static std::unique_ptr<Manager> instance;
  Manager();
};

}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored