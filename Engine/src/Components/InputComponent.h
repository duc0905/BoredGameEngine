#pragma once
#include <functional>

#include "Component.h"
#include "../Input/IInput.h"
#include "../IGame.h"

class InputComponent :
    public Component
{
public:
    // Inherited via Component
    virtual const std::string& GetComponentName() override;

    void BindAction(const std::string& name, IInput::ActionCallback func);
    void BindRange(const std::string& name, IInput::RangeCallback func);
};