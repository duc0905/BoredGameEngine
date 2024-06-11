#include "FileContent.hpp"
#include <imgui.h>

FileContentWindow::FileContentWindow(Manifest& game, int width, int height, std::shared_ptr<Bored::FileSystem::File> file)
    : SubWindow(game, ProcessFileNameToTitle(file), width, height, false), current_file(file)
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
        ImGui::Text("%s", content.c_str());
    }
}

std::string FileContentWindow::ProcessFileNameToTitle(std::shared_ptr<Bored::FileSystem::File> file)
{
    if (file != nullptr)
    {
        return file->GetName() + "###1";
    }
    return std::string("No file selected") + "###1";
}

void FileContentWindow::OnSetup() {};
void FileContentWindow::OnShutdown() {};

void FileContentWindow::SetFileToDisplay(std::shared_ptr<Bored::FileSystem::File> file)
{
    current_file = file;
    SetTitle(ProcessFileNameToTitle(file));
}
