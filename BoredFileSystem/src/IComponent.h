#pragma once
#include "BoredObject.h"
class IComponent :
    public BoredObject
{
public: 
    virtual BoredMap GetSavableData() = 0;
};

