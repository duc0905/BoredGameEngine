#include "STDFM.h"

#include "../../Adapter/STDFS.cpp"

namespace Bored {
namespace FileSystem {
namespace STDFS {
std::unique_ptr<Manager> Manager::instance = nullptr;

Manager& Manager::GetInstance() {
  if (instance == nullptr) {
    instance = std::unique_ptr<Manager>(new Manager());
  }

  return *instance;
}

std::shared_ptr<FileSystem::File> Manager::CreateFile(
    const std::string& filePath) {
  return std::make_shared<File>(filePath);
}

std::shared_ptr<FileSystem::Directory> Manager::MakeDirectory(
    const std::string& dirPath) {
  return std::make_shared<Directory>(dirPath);
}

Manager::Manager() { std::cout << "create"; };

}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored
