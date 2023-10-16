#include "FileExplorer.h"
#include <imgui.h>

namespace Bored
{
namespace Editor
{
FileExplorer::FileExplorer(int width, int height, OSAdapter::Window::WindowAPI& _win,
                           FileSystem::STDFS::Manager& _file_manager)
    : SubWindow("File Explorer", width, height), win(_win), file_manager(_file_manager)
{
}

FileExplorer::FileExplorer(int width, int height)
    : FileExplorer(width, height, OSAdapter::Window::WindowAPI::GetInstance(),
                   FileSystem::STDFS::Manager::GetInstance())
{
}

void FileExplorer::OnSetup()
{
}

bool FileExplorer::OnUpdate(double dt)
{
    return true;
}

void FileExplorer::DrawContent()
{
    if (ImGui::Button("Open folder"))
    {
        std::string dirPath = win.OpenDirPath();
        if (dirPath == "")
        {
            errorOpenDir = true;
        }
        else
        {
            loadedDir = true;
            current_dir = file_manager.MakeDirectory(dirPath);
        }
    }
    OpenWarningWindow();
    if (loadedDir)
    {
        DirectoryDisplayer();
    }
}

void FileExplorer::OnShutdown()
{
}

void FileExplorer::OpenWarningWindow()
{
    if (errorOpenDir)
    {
        ImGui::Begin("Error open directory",
                     &errorOpenDir); // Pass a pointer to our bool variable (the
                                     // window will have a closing button that
                                     // will clear the bool when clicked)
        ImGui::Text("Warning!: Error while open directory! Please try again");
        if (ImGui::Button("Close"))
            errorOpenDir = false;
        ImGui::End();
    }
}
void FileExplorer::DirectoryDisplayer()
{
    SubDirDisplayer(current_dir);
};

void FileExplorer::SubDirDisplayer(std::shared_ptr<Bored::FileSystem::Directory> dir)
{
    std::string dirname = dir->GetDirname();
    std::vector<std::shared_ptr<Bored::FileSystem::Directory>> subDirs = dir->GetDirectories();
    if (ImGui::TreeNode(dirname.c_str()))
    {
        for (std::shared_ptr<Bored::FileSystem::Directory> subDir : subDirs)
        {
            SubDirDisplayer(subDir);
        }
        std::vector<std::shared_ptr<Bored::FileSystem::File>> files = dir->GetFiles();
        for (std::shared_ptr<Bored::FileSystem::File> f : files)
        {
            FileDisplayer(f);
        }
        ImGui::TreePop();
    }
};

void FileExplorer::FileDisplayer(std::shared_ptr<Bored::FileSystem::File> file)
{
    std::string fname = file->GetName();
    if (ImGui::Selectable(fname.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
        if (ImGui::IsMouseDoubleClicked(0))
        {
            std::cout << "Double clicked on " << fname << std::endl;
            if (openFileCallBack != nullptr)
                openFileCallBack(file);
        };
}

void FileExplorer::SetOpenFileCallBack(std::function<void(std::shared_ptr<FileSystem::File>)> callBack)
{
    openFileCallBack = callBack;
}
} // namespace Editor
} // namespace Bored
