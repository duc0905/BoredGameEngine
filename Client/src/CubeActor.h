#pragma once
#include "Actor/Actor.h"
#include "Components/InputComponent.h"

class CubeActor :
    public Actor
{
public:
 
    virtual void OnAttach() override {
        std::cout << "DUk" << std::endl;
        auto inputComp = CreateComponent<InputComponent>();
//        inputComp->BindAction("yeet", std::bind(&CubeActor::Yell, this, std::placeholders::_1));
        inputComp->BindRange("yeet_RANGE", std::bind(&CubeActor::YellRange, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rX", std::bind(&CubeActor::RotateX, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rY", std::bind(&CubeActor::RotateY, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rZ", std::bind(&CubeActor::RotateZ, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rX", std::bind(&CubeActor::RotateX, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rY", std::bind(&CubeActor::RotateY, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rZ", std::bind(&CubeActor::RotateZ, this, std::placeholders::_1, std::placeholders::_2));
    }

    void RotateY(KeyInput::Action action, float weight) {
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(0, glm::pi<float>() / 100 * weight, 0));
    }
    
    void RotateX(KeyInput::Action action, float weight) {
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, 0, 0));
    }

    void RotateZ(KeyInput::Action action, float weight) {
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(0, 0, glm::pi<float>() / 100 * weight));
    }

    void Yell(KeyInput::Action action) {
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * 0.1f, glm::pi<float>() / 70 * 0.11f, glm::pi<float>() / 50 * 0.1f));
    }

    void YellRange(KeyInput::Action action, float weight) {
        auto transcomp = FindComponent<TransformComponent>();
        auto rotation = transcomp->GetRotation();
        transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, glm::pi<float>() / 70 * weight, glm::pi<float>() / 50 * weight));
    }


    virtual void OnUpdate(float dt) override{
        
    }
};
