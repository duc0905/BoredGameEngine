#include "GameLoop.hpp"
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
        std::chrono::steady_clock::time_point start, end;

        while (isRunning) {
            start = std::chrono::steady_clock::now();
            for (auto mod : mods) {
                end = std::chrono::steady_clock::now();
                dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                isRunning &= mod->OnUpdate(dt);
            }
        }

        for (auto mod : mods) {
            mod->OnShutdown();
        }
    }
}  // namespace Bored
