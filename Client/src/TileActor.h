#pragma once
#include "Actor/Actor.h"
#include "Renderer/Mesh.h"
#include "Components/MeshComponent.h"

class TileActor :
    public Actor
{
public:
    static std::shared_ptr<Mesh> whiteMesh_;
    static std::shared_ptr<Mesh> blackMesh_;

    TileActor(bool isBlack);
    virtual void OnTick(float dt) override {
        //LOG(GetID());
    }

    virtual void OnAttach() override {

    }


    void OnClick() override {
        std::cout << "This is a chess piece";
        auto transComp = FindComponent<TransformComponent>();
        std::cout << GetBoardPosition().first;
        std::cout << GetBoardPosition().second;
    }

    std::pair<int, int> GetBoardPosition() {
        auto transComp = FindComponent<TransformComponent>();
        return { transComp->GetTranslation()[0] / 2, transComp->GetTranslation()[2] / 2 };
    }

};

