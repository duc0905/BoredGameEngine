#include "FileManager.h"
#include "STDFS.h"
#include <memory>

namespace Bored {
namespace FileSystem {

std::shared_ptr<File> Bored::FileSystem::FileManager::CreateFile(
    const std::string& filename) {
  return std::make_shared<STDFS::File>(filename);
}

std::shared_ptr<Directory> FileManager::MakeDirectory(
    const std::string& dirname) {
  return std::make_shared<STDFS::Directory>();
}

}  // namespace FileSystem
}  // namespace Bored
