#pragma once
#include <memory>
#include "../Scene/Scene.hpp"

namespace Bored {
class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void OnUpdate(double dt, std::shared_ptr<Scene> scene) = 0;
};
} // namespace Bored
