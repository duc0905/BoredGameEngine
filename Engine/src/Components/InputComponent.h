#pragma once
#include <functional>

#include "Component.h"
#include "../Input/Input.h"

class InputComponent :
    public Component
{
public:
    // Inherited via Component
    virtual const std::string& GetComponentName() override;

    virtual void Update(double dt) override;

    void BindAction(const std::string& name, Input::ActionCallback func);
    void BindRange(const std::string& name, Input::RangeCallback func);
};