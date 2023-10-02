#pragma once
#include <memory>
#include <vector>

namespace Bored {
class Module {
 public:
  virtual void OnSetup() = 0;
  virtual bool OnUpdate(double dt) = 0;
  virtual void OnShutdown() = 0;
};

class GameLoop {
 public:
  GameLoop();
  ~GameLoop();

  void Run();
  void AddModule(std::shared_ptr<Module> module);

 private:
  std::vector<std::shared_ptr<Module>> mods;
};
}  // namespace Bored
