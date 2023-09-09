#pragma once
#include "../Adapter/FileSystem.h"
#include <memory>
#include <string>
#include <exception>

namespace Bored {
namespace FileSystem {

class FileManager {
 private:
  static std::shared_ptr<File> copyFile;
  std::shared_ptr<Directory> curDirector;

 public:
  std::string path;
  FileManager(std::string const& p) { path = p; }
  ~FileManager();

  std::shared_ptr<File> CreateFile(const std::string& filename);

  std::shared_ptr<Directory> MakeDirectory(const std::string& dirname);

  void DeteleFile() throw(std::exception);

  void RemoveDirectory() throw(std::exception);

  void CutFile(std::shared_ptr<File> file);

  void CopyFile(std::shared_ptr<File> file);

  std::shared_ptr<File> GetCopyFile() { return copyFile; };

  void LoadDirectory(std::string const& new_path) throw(std::exception);
};
}  // namespace FileSystem
}  // namespace Bored