#pragma once
#include "Module.h"
class InputModule : public Module
{
    // Inherited via Module
    virtual void OnMessage(Message& msg) override;
private:
    void OnKeyInput();
public:
    //InputModule();
};
