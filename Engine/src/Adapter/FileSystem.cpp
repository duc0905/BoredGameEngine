#include "FileSystem.h"

namespace Bored {
namespace FileSystem {
File::File(const std::string& p, const std::string& n) : path(p), name(n){};

File::File() : path(""), name(""){};

void File::SetName(const std::string& n) { name = n; }

void File::SetPath(const std::string& p) { path = p; }

std::string File::GetExtension() const {
  size_t dotPosition = name.find_last_of(".");

  if (dotPosition != std::string::npos) {
    // Extract the extension (substring after the last dot)
    return name.substr(dotPosition + 1);
  } else {
    // If no dot is found, return an empty string to indicate no extension
    return "";
  }
}

Directory::Directory(const std::string& dirPath) : path(dirPath) {
  size_t lastSeparator = dirPath.find_last_of(
      "/\\");  // Use '/' for Unix-like systems and '\\' for Windows

  if (lastSeparator != std::string::npos) {
    // Extract the directory name (substring before the last separator)
    dirName = dirPath.substr(lastSeparator + 1, -1);
  } else {
    // If no separator is found, return an empty string to indicate that
    dirName = "";
    throw std::exception("Invalid path for directory");
  }
}

}  // namespace FileSystem
}  // namespace Bored
