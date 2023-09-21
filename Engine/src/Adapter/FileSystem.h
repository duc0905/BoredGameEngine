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

  //   virtual bool IsExists() = 0;

  virtual void Rename(std::string const& newName) = 0;
  virtual void Delete() = 0;

  virtual std::vector<char> GetData() = 0;
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

class Directory {
 public:
  std::string path;
  std::string dirName;

  std::string GetDirname() const { return dirName; };
  std::string GetPath() const { return path; };

  Directory(const std::string& dirPath);

  //   virtual bool IsExists() = 0;
  virtual std::vector<std::shared_ptr<File>> GetFiles() const { return files; }
  virtual std::vector<std::shared_ptr<Directory>> GetDirectories() {
    return subDirectories;
  };

 protected:
  std::vector<std::shared_ptr<File>> files;
  std::vector<std::shared_ptr<Directory>> subDirectories;
};
}  // namespace FileSystem
}  // namespace Bored