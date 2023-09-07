#include "STDFS.h"

namespace fs = std::filesystem;

namespace Bored {
namespace FileSystem {
namespace STDFS {
File::File(const std::string& path) {
  fs::path filePath(path);

  std::string parentDir = filePath.parent_path().string();
  std::string filename = filePath.filename().string();

  std::ifstream inputFile(path, std::ios::binary);

  if (!inputFile.is_open()) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
  }

  // Create a vector to store the file content
  std::vector<char> fileContent;

  // Read the file content and push it into the vector
  char ch;
  while (inputFile.get(ch)) {
    fileContent.push_back(ch);
  }

  // Close the file
  inputFile.close();

  SetName(filename);
  SetPath(parentDir);
  SetContent(fileContent);
}

void File::Rename(std::string const& newName) {
  std::cout << "Hi" << std::endl;
}
void File::Delete() {}
void File::AppendData(std::vector<char> const&) {}
void File::WriteData(std::vector<char> const&) {}
void File::CopyFile() {}
}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored