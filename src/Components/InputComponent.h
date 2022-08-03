#pragma once
#include <functional>

#include "Component.h"
#include "../Input/Input.h"

class InputComponent :
    public Component
{
    // Inherited via Component
    virtual const std::string& GetComponentName() override;

    virtual void Update(double dt) override;

    void BindAction(const std::string& name, Input::ActionCallback);
    void BindRange(const std::string& name, Input::RangeCallback);
};