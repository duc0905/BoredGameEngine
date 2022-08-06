#pragma once
#include "../src/Actor/Actor.h"

class CubeActor :
    public Actor
{
public:

    virtual void OnAttach() override {
        std::cout << "DUk" << std::endl;
    }

    virtual void OnUpdate(float dt) override{
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>()/100*dt, glm::pi<float>()/70*dt, glm::pi<float>() / 50 * dt));
    }
};

