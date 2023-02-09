#pragma once
#include "Actor\Actor.h"
#include "PissActor.h"
#include "Components/InputComponent.h"
#include "Components/MeshComponent.h"

class Pawn: public PissActor
{   
public:
    static std::shared_ptr<Mesh> mesh_;

    Pawn(int x, int y, int team);
    virtual void OnTick(float dt) override {
        //LOG(GetID());
    }

    virtual void OnAttach() override {

    }

    void OnClick() override {
        std::cout << "DJT ME MAY";
    }

    // Inherited via PissActor
    inline virtual Type getType() override {
        return PissActor::Type::PAWN;
    };
};

