#pragma once
#include <memory>
#include <string>
#include <vector>

namespace Bored {
namespace FileSystem {

class File {
 public:
  std::string path;
  std::string name;
  File();
  File(const std::string& p, const std::string& n);

  std::string GetName() const { return name; };
  std::string GetPath() const { return path; };
  virtual void Rename(std::string const& newName) = 0;
  virtual void Delete() = 0;

  virtual void AppendData(std::vector<char>&) = 0;
  virtual void WriteData(std::vector<char>&) = 0;

  virtual void AppendData(const std::string& data) = 0;
  virtual void WriteData(const std::string& data) = 0;

  virtual std::size_t GetSize() const = 0;
  std::string GetExtension() const;

 protected:
  void SetName(const std::string&);
  void SetPath(const std::string&);
};

class Directory : public File {
 public:
  std::vector<std::shared_ptr<File>> files;
  std::vector<std::shared_ptr<Directory>> subDirectories;

  std::vector<std::shared_ptr<File>> GetFiles() const { return files; }
  std::vector<std::shared_ptr<Directory>> GetDirectories() const {
    return subDirectories;
  }
};
}  // namespace FileSystem
}  // namespace Bored