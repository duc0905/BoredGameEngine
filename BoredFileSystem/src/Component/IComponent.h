#pragma once
#include "../BoredObject.h"
class IComponent :
    public BoredObject
{
public: 
    virtual const std::string& GetComponentName() = 0;
    virtual void Update(double dt) {};
    virtual BoredMap Serialize() = 0;
};

