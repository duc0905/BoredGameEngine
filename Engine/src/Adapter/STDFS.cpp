#include "STDFS.h"

namespace fs = std::filesystem;

namespace Bored {
namespace FileSystem {
namespace STDFS {
File::File(const std::string& path) {
  fs::path filePath(path);

  std::string parentDir = filePath.parent_path().string();
  std::string filename = filePath.filename().string();

  std::fstream file(path, std::ios::binary | std::ios::out);

  if (!file.is_open()) {
    std::cerr << "Failed to open/create file: " << filePath << std::endl;
  }

  file.close();

  SetName(filename);
  SetPath(parentDir);
}

void File::Rename(std::string const& newName) {
  fs::path cwd = path;
  fs::path target = cwd / newName;
  fs::path source = cwd / name;
  if (fs::exists(target)) {
    std::cerr << "File already exist";
    return;
  };
  if (fs::exists(source)) {
    try {
      // Rename the file
      fs::rename(source, target);
      std::cout << "File renamed successfully." << std::endl;
    } catch (const fs::filesystem_error& e) {
      std::cerr << "Error renaming file: " << e.what() << std::endl;
      throw std::exception("Error renaming files");
    }
  } else {
    std::cerr << "Source file does not exist." << std::endl;
    throw std::exception("Error renaming files");
  }
}

void File::Delete() {
  fs::path cwd = path;
  fs::path fileToDelete = cwd / name;
  try {
    fs::remove(fileToDelete);
    std::cout << "File deleted successfully." << std::endl;
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Waring: " << e.what() << std::endl;
  }
}

void File::AppendData(const std::string& data) {
  std::vector<char> charVector(data.begin(), data.end());
  AppendData(charVector);
}

void File::WriteData(const std::string& data) {
  std::vector<char> charVector(data.begin(), data.end());
  WriteData(charVector);
}

void File::AppendData(std::vector<char>& data) {
  fs::path cwd = path;
  fs::path filePath = cwd / name;
  std::ofstream outputFile(filePath, std::ios::binary | std::ios::app);

  if (!outputFile.is_open()) {
    std::cerr << "Failed to open file for appending: " << filePath << std::endl;
    std::string ex = "Unable to append data to file: " + filePath.string();
    throw std::exception(ex.c_str());
  }

  // Append the data to the file
  outputFile.write(data.data(), data.size());

  // Close the file
  outputFile.close();

  std::cout << "Data appended to the file successfully." << std::endl;
}

void File::WriteData(std::vector<char>& data) {
  fs::path cwd = path;
  fs::path filePath = cwd / name;
  std::ofstream outputFile(filePath, std::ios::binary | std::ios::trunc);

  if (!outputFile.is_open()) {
    std::cerr << "Failed to open file for appending: " << filePath << std::endl;
    std::string ex = "Unable to overwrite data to file: " + filePath.string();
    throw std::exception(ex.c_str());
  }

  // Append the data to the file
  outputFile.write(data.data(), data.size());

  // Close the file
  outputFile.close();

  std::cout << "Data overwrite to the file successfully." << std::endl;
};

std::size_t File::GetSize() const { return 0; };

// bool File::IsExists

Directory::Directory(const std::string& file_path)
    : FileSystem::Directory(file_path) {
  fs::path cwd = file_path;
  try {
    // Use std::filesystem::create_directory to create the directory
    fs::create_directory(file_path);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  for (const auto& entry : fs::directory_iterator(cwd)) {
    const fs::path& currentPath = entry.path();

    if (fs::is_directory(currentPath)) {
      std::cout << "Folder: " << currentPath.filename() << std::endl;
    } else if (fs::is_regular_file(currentPath)) {
      std::cout << "File: " << currentPath.filename() << std::endl;
    }
    // LOAD file and sub dir
  };
  //   std::cout << "Success";
}
}  // namespace STDFS
}  // namespace FileSystem
}  // namespace Bored