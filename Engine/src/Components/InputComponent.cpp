
#include "../pch.h"
#include "InputComponent.h"

const std::string& InputComponent::GetComponentName()
{
    return "Input";
}

void InputComponent::BindAction(const std::string& name, IInput::ActionCallback func)
{
    IGame::GetInput().BindAction(name, func);
}

void InputComponent::BindRange(const std::string& name, IInput::RangeCallback func)
{
    IGame::GetInput().BindRange(name, func);
}
