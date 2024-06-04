#include "FileContent.h"
#include <imgui.h>

namespace Bored
{
namespace Editor
{

FileContentWindow::FileContentWindow(int width, int height, std::shared_ptr<FileSystem::File> file)
    : SubWindow(ProcessFileNameToTitle(file), width, height, false), current_file(file)
{
}

bool FileContentWindow::OnUpdate(double dt)
{
    return true;
}

void FileContentWindow::DrawContent()
{
    if (current_file != nullptr)
    {
        auto data = current_file->GetData();
        std::string content = std::string(begin(data), end(data));
        ImGui::Text(content.c_str());
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

void FileContentWindow::OnSetup() {};
void FileContentWindow::OnShutdown() {};

void FileContentWindow::SetFileToDisplay(std::shared_ptr<FileSystem::File> file)
{
    current_file = file;
    SetTitle(ProcessFileNameToTitle(file));
}
} // namespace Editor
} // namespace Bored
