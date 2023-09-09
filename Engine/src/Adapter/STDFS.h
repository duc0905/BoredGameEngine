#pragma once
#include "FileSystem.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace Bored {
namespace FileSystem {
namespace STDFS {
class File : public FileSystem::File {
 public:
  File(const std::string& path);

  // Inherited via File
  virtual void Rename(std::string const& newName) override;
  virtual void Delete() override;
  
  virtual void AppendData(std::vector<char>& data) override;
  virtual void WriteData(std::vector<char>& data) override;

  virtual void AppendData(const std::string& data) override;
  virtual void WriteData(const std::string& data) override;

 private:
  std::fstream stream;
};
class Directory : public FileSystem::Directory {};

}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored