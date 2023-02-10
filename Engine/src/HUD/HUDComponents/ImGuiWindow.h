#pragma once
#include <imgui.h>

#include "IHUDWindow.h"

class ImGuiHUDWindow : public IHUDWindow {
 private:
  std::string title_ = "Debug window";
  bool isOpen_ = true;

 public:
  // Inherited via IHUDWindow
  virtual bool Render() override;

  void SetTitle(const std::string& title) { title_ = title; }
  const std::string& GetTitle() const { return title_; }
  void Open() { isOpen_ = true; }
  void Close() { isOpen_ = false; }
};
