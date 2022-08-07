#include "InputComponent.h"

const std::string& InputComponent::GetComponentName()
{
    return "Input";
}

void InputComponent::Update(double dt)
{
}

void InputComponent::BindAction(const std::string& name, Input::ActionCallback func)
{
    Input::GetInstance()->BindAction(name, func);
}

void InputComponent::BindRange(const std::string& name, Input::RangeCallback func)
{
    Input::GetInstance()->BindRange(name, func);
}
