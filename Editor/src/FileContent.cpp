#include "FileContent.h"

namespace Bored
{
namespace Editor
{

FileContentWindow::FileContentWindow(int width, int height, std::shared_ptr<FileSystem::File> file)
    : SubWindow(ProcessFileNameToTitle(file), width, height, false), current_file(file)
{
}

void FileContentWindow::OnUpdate()
{
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
    {
        SetOpen(false);
    }
}

std::string FileContentWindow::ProcessFileNameToTitle(std::shared_ptr<FileSystem::File> file)
{
    if (file != nullptr)
    {
        return file->GetName() + "###1";
    }
    return std::string("No file selected") + "###1";
}

void FileContentWindow::OnInit(){};
void FileContentWindow::OnShutdown(){};

void FileContentWindow::SetFileToDisplay(std::shared_ptr<FileSystem::File> file)
{
    current_file = file;
    SetTitle(ProcessFileNameToTitle(file));
}
} // namespace Editor
} // namespace Bored
