#pragma once
#include "Actor/Actor.h"
#include "Components/InputComponent.h"
#include "Components/MeshComponent.h"
#include "Components/AudioComponent.h"

class ChessBoardActor : public Actor
{
public:
    ChessBoardActor();
    virtual void OnTick(float dt) override {
        //LOG(GetID());
    }
    virtual void OnAttach() override {
        auto inputComp = CreateComponent<InputComponent>();
        inputComp->BindAction("yeet", std::bind(&ChessBoardActor::Yell, this, std::placeholders::_1));
        inputComp->BindAction("buonce", [this](KeyInput::Action action) -> void {
                auto audioComp = FindComponent<AudioComponent>();
                if (action == KeyInput::PRESS)
                {
                    audioComp->PlayCompSound();
                    LOG_COLOR("PRESS B", COLOR::GREEN, COLOR::BLACK);
                }
                audioComp->PlayCompSound();
            });
        inputComp->BindRange("yeet_RANGE", std::bind(&ChessBoardActor::YellRange, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rX", std::bind(&ChessBoardActor::RotateX, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rY", std::bind(&ChessBoardActor::RotateY, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("rZ", std::bind(&ChessBoardActor::RotateZ, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rX", std::bind(&ChessBoardActor::RotateX, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rY", std::bind(&ChessBoardActor::RotateY, this, std::placeholders::_1, std::placeholders::_2));
        inputComp->BindRange("-rZ", std::bind(&ChessBoardActor::RotateZ, this, std::placeholders::_1, std::placeholders::_2));
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
        //transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, glm::pi<float>() / 70 * weight, glm::pi<float>() / 50 * weight));
        //transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, glm::pi<float>() / 70 * weight, glm::pi<float>() / 50 * weight));
        //transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, glm::pi<float>() / 70 * weight, glm::pi<float>() / 50 * weight));
        transcomp->SetRotation(rotation + glm::vec3(glm::pi<float>() / 100 * weight, 0.0f, 0.0f));
    }

    void PlaySoundS(KeyInput::Action action) {
        auto audioComp = FindComponent<AudioComponent>();
        audioComp->PlayCompSound();
    }
};

