#pragma once
#include <string>
#include "Bored.hpp"
#include <iostream>

namespace Bored
{
namespace Editor
{
/// @brief Sub-window of editor
/// The OnUpdate method of this class should only update the logic and not contain the drawing
class SubWindow : public Window
{
  public:
    SubWindow() = default;
    SubWindow(const std::string& title, int width, int height, bool open)
        : _title(title), _width(width), _height(height), _open(open){};
    SubWindow(const std::string& title, int width, int height) : SubWindow(title, width, height, true){};

    virtual ~SubWindow(){};

    // Draw the window
    void Create();
    void SetOpen(bool open);

    void SetTitle(const std::string& title) override;
    int GetWidth() const override;
    int GetHeight() const override;
    void SetWidth(int w) override;
    void SetHeight(int h) override;
    void SetFullscreen(bool) override;

    void PollEvents() override {}
    Frontend::Input* GetInput() override
    {
        return nullptr;
    }

    void* GetNativeWindow() override;

  private:
    std::string _title;
    bool _open;
    int _width, _height;
};

} // namespace Editor
} // namespace Bored
