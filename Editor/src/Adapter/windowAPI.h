#pragma once
#include "api.h"
#include <iostream>
#include <memory>
#include <shlobj.h>
#include <windows.h>

namespace Bored {
namespace Editor {
namespace OSAdapter {
namespace Window {
class WindowAPI : public OSAdapter::OS {
 private:
  static std::unique_ptr<WindowAPI> _window;

 public:
  virtual std::string OpenDirPath() override;
  static WindowAPI& GetInstance();

 private:
  WindowAPI();
};
}  // namespace Window
}  // namespace OSAdapter
}  // namespace Editor
}  // namespace Bored