#pragma once
#include "Component.h"
class InputComponent :
    public Component
{
    // Inherited via Component
    virtual const std::string& GetComponentName() override;

    virtual void Update(double dt) override;
};