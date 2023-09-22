#include "GameLoop.h"
#include <chrono>

namespace Bored {
GameLoop::GameLoop() {}

GameLoop::~GameLoop() {}

void GameLoop::AddModule(std::shared_ptr<Module> module) {
  mods.push_back(module);
}
void GameLoop::Run() {
  for (auto mod : mods) {
    mod->OnSetup();
  }

  bool isRunning = true;
  double dt = 0;
  std::chrono::time_point<std::chrono::steady_clock> start, end;

  while (isRunning) {
    start = std::chrono::steady_clock::now();
    for (auto mod : mods) {
      isRunning &= mod->OnUpdate(dt);
    }
    end = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::milli> diff = end - start;
    dt = diff.count();
  }

  for (auto mod : mods) {
    mod->OnShutdown();
  }
}
}  // namespace Bored