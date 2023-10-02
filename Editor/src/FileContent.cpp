#include "FileContent.h"

namespace Bored {
namespace Editor {
// FileContentWindow::FileContentWindow(int width, int height)
//: BoredWindow(file_name.c_str(), width, height) {}

FileContentWindow::FileContentWindow(int width, int height,
                                     std::shared_ptr<FileSystem::File> file)
    : BoredWindow(ProcessFileNameToTitle(file), width, height, false),
      current_file(file) {}

void FileContentWindow::Update() {
  ImGui::Text("Hello from another window!");
  if (ImGui::Button("Close Me")) {
    SetOpen(false);
  }
}

std::string FileContentWindow::ProcessFileNameToTitle(
    std::shared_ptr<FileSystem::File> file) {
  if (file != nullptr) {
    return file->GetName() + "###1";
  }
  return std::string("No file selected") + "###1";
}

void FileContentWindow::Init(){};
void FileContentWindow::Render(){};
void FileContentWindow::Shutdown(){};

// fill all virtual class in BoredWindow
bool FileContentWindow::IsRunning() const { return 0; };

void FileContentWindow::SetFileToDisplay(
    std::shared_ptr<FileSystem::File> file) {
  current_file = file;
  SetTitle(ProcessFileNameToTitle(file));
}
}  // namespace Editor
}  // namespace Bored