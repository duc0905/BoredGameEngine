#pragma once
#include "FileSystem.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


namespace Bored {
namespace FileSystem {
namespace STDFS {
class File : public FileSystem::File {
 public:
  File(const std::string& path);

  // Inherited via File
  virtual void Rename(std::string const& newName) override;
  virtual void Delete() override;
  virtual void AppendData(std::vector<char> const&) override;
  virtual void WriteData(std::vector<char> const&) override;
  virtual void CopyFile() override;

 private:
  std::fstream stream;
};
class Directory : public FileSystem::Directory {};

}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored