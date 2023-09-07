#pragma once
#include <string>
#include <vector>
#include <memory>

namespace Bored {
    namespace FileSystem {

    class File {
      public:
      std::string path;
      std::string name;
      std::vector<char> content;

      std::string GetName() const { return name; };
      std::string GetPath() const { return path; };
      void Rename(const std::string& newName);
      void Delete();
      bool OpenFile();
      bool CloseFile();

      void AppendData(const std::vector<char>&);
      void WriteData(const std::vector<char>&);

      bool CopyFile();
      virtual std::size_t GetSize() const { return content.size(); };
      std::vector<std::string> GetExtensions() const;
    };

    class Directory : public File {
      public:
        std::vector<std::shared_ptr<File>> files;
        std::vector<std::shared_ptr<Directory>> directories;

        std::vector<std::shared_ptr<File>> GetFiles() const { return files; }
        std::vector<std::shared_ptr<Directory>> GetDirectories() const { return directories; }
    };
    } // namespace FileSystem
} // namespace Bored