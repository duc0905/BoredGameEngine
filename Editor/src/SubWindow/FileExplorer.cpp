#include "FileExplorer.hpp"
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <imgui.h>

FileExplorer::FileExplorer(Manifest& game, int width, int height, Bored::Editor::OSAdapter::Window::WindowAPI& _win, Bored::FileSystem::STDFS::Manager& _file_manager)
: SubWindow(game, "File Explorer", width, height), win(_win), file_manager(_file_manager)
{
}

FileExplorer::FileExplorer(Manifest& game, int width, int height)
: FileExplorer(game, width, height, Bored::Editor::OSAdapter::Window::WindowAPI::GetInstance(), Bored::FileSystem::STDFS::Manager::GetInstance())
{
}

void FileExplorer::OnSetup()
{
}

bool FileExplorer::OnUpdate(double dt)
{
    return true;
}

void FileExplorer::NewFrame() {
    if (ImGui::Button("Open folder")) {
        // std::string dirPath = win.OpenDirPath();
        // if (dirPath == "") {
        //     errorOpenDir = true;
        // }
        // else {
        //     loadedDir = true;
        //     current_dir = file_manager.MakeDirectory(dirPath);
        //     _openProject(dirPath);
        // }
        char* path = tinyfd_selectFolderDialog("Open Project", NULL);
        if (path != NULL) {
            loadedDir = true;
            current_dir = file_manager.MakeDirectory(path);
            _openProject(path);
        }
    }

    if (errorOpenDir) OpenWarningWindow();
    if (loadedDir) DirectoryDisplayer();
}

void FileExplorer::OnShutdown()
{
}

void FileExplorer::OpenWarningWindow()
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

void FileExplorer::SetOpenFileCallBack(std::function<void(std::shared_ptr<Bored::FileSystem::File>)> callBack)
{
    openFileCallBack = callBack;
}

void FileExplorer::SetOpenProjectCallback(std::function<void(std::string)> cb) {
    _openProject = cb;
}
