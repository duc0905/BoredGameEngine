#include <imgui.h>
#include <imgui_internal.h>
#include "SubWindow.hpp"

void SubWindow::Create()
{
    if (_open)
    {
        ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Once);
        ImGui::Begin(_title.c_str(), &_open, 0);
        auto dim = ImGui::GetWindowSize();
        _width = dim.x;
        _height = dim.y;
        DrawContent();
        ImGui::End();
    }
}

void SubWindow::SetOpen(bool open)
{
    _open = open;
};

void SubWindow::SetTitle(const std::string& title)
{
    _title = title;
};

int SubWindow::GetWidth() const
{
    return _width;
}

int SubWindow::GetHeight() const
{
    return _height;
}

void SubWindow::SetWidth(int w) {
    _width = w;
    float ww = _width;
    float hh = _height;
    ImGui::SetWindowSize({ww, hh});
}

void SubWindow::SetHeight(int h) {
    _height = h;
    float ww = _width;
    float hh = _height;
    ImGui::SetWindowSize({ww, hh});
}

void SubWindow::SetFullscreen(bool f)
{
  throw std::exception("Cannot a sub-window fullscreen");
}
void* SubWindow::GetNativeWindow()
{
    return ImGui::FindWindowByName(_title.c_str());
}
