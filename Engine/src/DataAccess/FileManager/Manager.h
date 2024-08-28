#pragma once
#include <exception>
#include <memory>
#include <string>
#include "../../Adapter/FileSystem.h"

namespace Bored {
namespace FileSystem {

class FileManager {
 public:
  virtual std::shared_ptr<File> CreateFile(const std::string& filePath) = 0;

  virtual std::shared_ptr<Directory> MakeDirectory(
      const std::string& dirPath) = 0;
};
}  // namespace FileSystem
}  // namespace Bored
