#include "FileExplorer.h"

namespace Bored {
namespace Editor {
FileExplorer::FileExplorer(int width, int height,
                           OSAdapter::Window::WindowAPI& _win,
                           FileSystem::STDFS::Manager& _file_manager)
    : BoredWindow("File Explorer", width, height),
      win(_win),
      file_manager(_file_manager) {}
FileExplorer::FileExplorer(int width, int height)
    : FileExplorer(width, height, OSAdapter::Window::WindowAPI::GetInstance(),
                   FileSystem::STDFS::Manager::GetInstance()) {}
void FileExplorer::Init() {}
void FileExplorer::Update() {
  bool openWarning = false;
  static float f = 0.0f;
  static std::string a = "Explorer";

  ImGui::Text("%s", a.c_str());
  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  ImGui::Checkbox("Another Window", &errorOpenDir);
  if (ImGui::Button("Open folder")) {
    std::string dirPath = win.OpenDirPath();
    if (dirPath == "") {
      errorOpenDir = true;
    } else {
      loadedDir = true;
      current_dir = file_manager.MakeDirectory(dirPath);
    }
  }
  OpenWarningWindow();
  if (loadedDir) {
    DirectoryDisplayer();
    if (openContentWindow) {
      ContentWindow();
    }
  }
}
void FileExplorer::DirectoryDisplayer() { SubDirDisplayer(current_dir); };

void FileExplorer::FileDisplayer(
    std::shared_ptr<Bored::FileSystem::File> file) {
  std::string fname = file->GetName();
  if (ImGui::Selectable(fname.c_str(), false,
                        ImGuiSelectableFlags_AllowDoubleClick))
    if (ImGui::IsMouseDoubleClicked(0)) {
      openContentWindow = true;
      std::cout << "Double clicked on " << fname << std::endl;
    };
}

void FileExplorer::ContentWindow() {
  ImGui::Begin("Content");
  ImGui::Text("Hello from another window!");
  if (ImGui::Button("Close Me")) openContentWindow = false;
  ImGui::End();
}

void FileExplorer::SubDirDisplayer(
    std::shared_ptr<Bored::FileSystem::Directory> dir) {
  std::string dirname = dir->GetDirname();
  std::vector<std::shared_ptr<Bored::FileSystem::Directory>> subDirs =
      dir->GetDirectories();
  if (ImGui::TreeNode(dirname.c_str())) {
    for (std::shared_ptr<Bored::FileSystem::Directory> subDir : subDirs) {
      SubDirDisplayer(subDir);
    }
    std::vector<std::shared_ptr<Bored::FileSystem::File>> files =
        dir->GetFiles();
    for (std::shared_ptr<Bored::FileSystem::File> f : files) {
      FileDisplayer(f);
    }
    ImGui::TreePop();
  }
};

void FileExplorer::Render() {}
void FileExplorer::Shutdown() {}
bool FileExplorer::IsRunning() const { return 0; };

void FileExplorer::OpenWarningWindow() {
  if (errorOpenDir) {
    ImGui::Begin("Error open directory",
                 &errorOpenDir);  // Pass a pointer to our bool variable (the
                                  // window will have a closing button that
                                  // will clear the bool when clicked)
    ImGui::Text("Warning!: Error while open directory! Please try again");
    if (ImGui::Button("Close")) errorOpenDir = false;
    ImGui::End();
  }
}
}  // namespace Editor
}  // namespace Bored