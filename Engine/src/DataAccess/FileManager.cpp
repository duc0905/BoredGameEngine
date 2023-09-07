#include "FileManager.h"

namespace Bored {
namespace FileSystem {

std::shared_ptr<File> Bored::FileSystem::FileManager::CreateFile(
    std::string const& filename) {
  return std::shared_ptr<File>(filename);
}

std::shared_ptr<Directory> FileManager::MakeDirectory(
    std::string const& dirname) {
  return std::shared_ptr<Directory>();
}

}  // namespace FileSystem
}  // namespace Bored
