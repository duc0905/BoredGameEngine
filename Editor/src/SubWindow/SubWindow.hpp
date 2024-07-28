#pragma once
#include <string>
#include "../GameManifest/Manifest.hpp"
#include "Bored.hpp"

/// @brief Sub-window of editor
/// The OnUpdate method of this class should only update the logic and not contain the drawing
class SubWindow : public Bored::Window
{
  SubWindow() = delete;
public:
  SubWindow(Manifest& game) : _game(game) {};
  SubWindow(Manifest& game, const std::string& title, int width, int height, bool open)
  : _game(game), _title(title), _width(width), _height(height), _open(open){};
  SubWindow(Manifest& game, const std::string& title, int width, int height) : SubWindow(game, title, width, height, true){};

  virtual ~SubWindow(){};

  // Draw the window
  void Create();
  void SetOpen(bool open);

  // void NewFrame() override {}

  void SetTitle(const std::string& title) override;
  int GetWidth() const override;
  int GetHeight() const override;
  void SetWidth(int w) override;
  void SetHeight(int h) override;
  void SetFullscreen(bool) override;

  void PollEvents() override {}
  Bored::Frontend::Input* GetInput() override
  {
    return nullptr;
  }

  void* GetNativeWindow() override;

private:
  std::string _title;
  bool _open;
  int _width, _height;

protected:
  Manifest& _game;
};
