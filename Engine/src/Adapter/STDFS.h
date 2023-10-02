#pragma once
#include "FileSystem.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace Bored {
namespace FileSystem {
namespace STDFS {
namespace fs = std::filesystem;
class File : public FileSystem::File {
 public:
  File(const std::string& path);

  // Inherited via File
  virtual void Rename(std::string const& newName) override;
  virtual void Delete() override;
  //   virtual bool IsExists() override;

  virtual void AppendData(std::vector<char>& data) override;
  virtual void WriteData(std::vector<char>& data) override;

  virtual void AppendData(const std::string& data) override;
  virtual void WriteData(const std::string& data) override;

  virtual std::vector<char> GetData() override;

  virtual std::size_t GetSize() const override;

 private:
  std::fstream stream;
};
class Directory : public FileSystem::Directory {
 private:
  std::vector<std::string> dirPaths = std::vector<std::string>();
  bool loaded = false;

 public:
  Directory(const std::string& file_path);
  void LoadSubDirectories();
  std::vector<std::shared_ptr<FileSystem::Directory>> GetDirectories() override;
};
}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored
