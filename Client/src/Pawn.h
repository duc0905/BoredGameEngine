#pragma once
#include "Actor\Actor.h"
#include "PissActor.h"
#include "Components/InputComponent.h"
#include "Components/MeshComponent.h"

class Pawn: public PissActor
{   
public:

    Pawn();
    virtual void OnTick(float dt) override {
        //LOG(GetID());
    }

    virtual void OnAttach() override {

    }

    // Inherited via PissActor
    virtual Type getId() override;
    virtual std::vector<std::pair<int, int>> getPossileMoves() override;
};

