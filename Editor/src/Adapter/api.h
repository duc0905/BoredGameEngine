#pragma once
#include <iostream>

namespace Bored {
namespace Editor {
namespace OSAdapter {
class OS {
 public:
  virtual std::string OpenDirPath() = 0;
};
}  // namespace OSAdapter
}  // namespace Editor
}  // namespace Bored