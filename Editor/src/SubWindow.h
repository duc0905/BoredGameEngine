#pragma once
#include <imgui.h>
#include <string>

namespace Bored
{
namespace Editor
{
class SubWindow
{
  public:
    SubWindow() = default;
    SubWindow(const std::string& title, int width, int height, bool open)
        : _title(title), _width(width), _height(height), _open(open){};
    SubWindow(const std::string& title, int width, int height) : SubWindow(title, width, height, true){};
    virtual ~SubWindow(){};

    void Create()
    {
        if (_open)
        {
            ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Once);
            ImGui::Begin(_title.c_str(), &_open, 0);
            OnUpdate();
            ImGui::End();
        }
    }

    void SetOpen(bool open)
    {
        _open = open;
    };
    void SetTitle(std::string title)
    {
        _title = title;
    };

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnShutdown() = 0;

  private:
    std::string _title;
    bool _open;
    int _width, _height;
};

} // namespace Editor
} // namespace Bored
