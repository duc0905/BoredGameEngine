#pragma once
#include <imgui.h>
#include <string>

namespace Bored
{
namespace Editor
{
class Window
{
  public:
    Window() = default;
    Window(std::string title, int width, int height, bool open)
        : _title(title), _width(width), _height(height), _open(open){};
    Window(std::string title, int width, int height) : Window(title, width, height, true){};
    virtual ~Window(){};

    void Create()
    {
        if (_open)
        {
            ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Once);
            ImGui::Begin(_title.c_str(), &_open, 0);
            Update();
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

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;

  private:
    std::string _title;
    bool _open;
    int _width, _height;
};

} // namespace Editor
} // namespace Bored
