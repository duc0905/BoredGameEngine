#include "FileManager.h"
#include "STDFS.h"
#include <memory>

namespace Bored {
namespace FileSystem {

void FileManager::OpenWorkingDirectory(const std::string& openPath) {}

std::shared_ptr<File> FileManager::CreateFile(const std::string& filename) {
#if FILE_SYSTEM_API == "STDFS"
  return std::make_shared<STDFS::File>(filename);
#else
  std::cerr << "Please use a valid FILE_SYSTEM_API" << std::endl;
  return nullptr;
#endif
}

std::shared_ptr<Directory> FileManager::MakeDirectory(
    const std::string& dirname) {
#if FILE_SYSTEM_API == "STDFS"
  return std::make_shared<STDFS::Directory>();
#else
  std::cerr << "Please use a valid FILE_SYSTEM_API" << std::endl;
  return nullptr;
#endif
}

}  // namespace FileSystem
}  // namespace Bored
