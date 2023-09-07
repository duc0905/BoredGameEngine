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
  std::vector<char> content;
  File();
  File(const std::string& p, const std::string& n,
       std::vector<char>& c);

  std::string GetName() const { return name; };
  std::string GetPath() const { return path; };
  virtual void Rename(std::string const& newName) = 0;
  virtual void Delete() = 0;
  virtual void CloseFile() = 0;

  virtual void AppendData(std::vector<char> const&) = 0;
  virtual void WriteData(std::vector<char> const&) = 0;

  virtual void CopyFile() = 0;
  std::size_t GetSize() const { return content.size(); };
  std::vector<std::string> GetExtensions() const;

  protected:
  void SetName(const std::string&);
  void SetPath(const std::string&);
  void SetContent(std::vector<char>&);
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