#pragma once
#include <memory>
#include <vector>

namespace Bored {

// TODO: Consider seperating Logic modules and frontend modules
class Module {
public:
    virtual void OnSetup() = 0;
    virtual bool OnUpdate(double dt) = 0;
    virtual void OnShutdown() = 0;
};

class GameLogic {
public:
    GameLogic();
    ~GameLogic();

    // void Run();
    void AddModule(std::shared_ptr<Module> module);
    bool OnUpdate(double dt);

private:
    std::vector<std::shared_ptr<Module>> mods;
};
}  // namespace Bored
